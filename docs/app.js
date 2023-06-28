// Load the WASM module
Module.onRuntimeInitialized = function () {
  let ctx = document.getElementById("canvas").getContext("2d");

  let move = "", botcolor = 1, turn = 0, click = 0, x0, y0, x1, y1, moves = 0, 
  board = [[30,10,20,50,40,21,11,31],[1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
  [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[-30,-10,-20,-50,-40,-21,-11,-31]], 
  positions = [[[30,10,20,50,40,21,11,31],[1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
  [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[-30,-10,-20,-50,-40,-21,-11,-31]]],
  enpassant = -1, piece_positions = [];
  //pawns, knights, bishops, rooks, queens and kings (W,B)
  let pieces = [[8,8],[2,2],[2,2],[2,2],[1,1],[1,1]];
  let castled = 1; //%2 == 0 if white has castled, %3 == 0 if black has castled
  let kingmoved = 1; //%2 == 0 if white king has moved, %3 == 0 if black king has moved
  //black left, right - white left, right
  let rookmoved = [[0,0],[0,0]];
  let imagecount = 13;
  let loaded_images = 0;

  let boardimg = new Image();
  boardimg.src = "./Images/board.png";
  boardimg.onload = images_loaded;
  let wpawnimg = new Image();
  wpawnimg.src = "./Images/whitepawn2.png";
  wpawnimg.onload = images_loaded;
  let wknightimg = new Image();
  wknightimg.src = "./Images/whiteknight2.png";
  wknightimg.onload = images_loaded;
  let wbishopimg = new Image();
  wbishopimg.src = "./Images/whitebishop2.png";
  wbishopimg.onload = images_loaded;
  let wrookimg = new Image();
  wrookimg.src = "./Images/whiterook2.png";
  wrookimg.onload = images_loaded;
  let wqueenimg = new Image();
  wqueenimg.src = "./Images/whitequeen2.png";
  wqueenimg.onload = images_loaded;
  let wkingimg = new Image();
  wkingimg.src = "./Images/whiteking2.png";
  wkingimg.onload = images_loaded;
  let bpawnimg = new Image();
  bpawnimg.src = "./Images/blackpawn2.png";
  bpawnimg.onload = images_loaded;
  let bknightimg = new Image();
  bknightimg.src = "./Images/blackknight2.png";
  bknightimg.onload = images_loaded;
  let bbishopimg = new Image();
  bbishopimg.src = "./Images/blackbishop2.png";
  bbishopimg.onload = images_loaded;
  let brookimg = new Image();
  brookimg.src = "./Images/blackrook2.png";
  brookimg.onload = images_loaded;
  let bqueenimg = new Image();
  bqueenimg.src = "./Images/blackqueen2.png";
  bqueenimg.onload = images_loaded;
  let bkingimg = new Image();
  bkingimg.src = "./Images/blackking2.png";
  bkingimg.onload = images_loaded;

  function images_loaded(){
    loaded_images++;
    if(loaded_images == imagecount){
      drawBoard();
    }
  }

  // Interact with the C++ chess bot using ccall or cwrap
  const movepiece = Module.cwrap('movepiece', 'number', ['number', 'number', 'number', 'number', 'number'
  , 'string', 'number', 'string', 'string', 'number', 'number', 'string']);
  const gameend = Module.cwrap('gameend', 'number', ['number', 'number', 'string', 'string'
  , 'string', 'string', 'number', 'number', 'string']);
  let basicbot;

  let openingbook;

  async function loadBinaryData(url) {
    const response = await fetch(url);
    const buffer = await response.arrayBuffer();
    const data = new Uint8Array(buffer);
    return data;
  }

  function convert_move(move_string){
    let i = 1;
    let result = [];
    for(let j = 0; j < 5; j++){
      let element = "";
      while(true){
        if(move_string[i] == ',' || move_string[i] == '[' || move_string[i] == ']'){
          i++;
        }else{
          break;
        }
      }
      while(true){
        if(move_string[i] == ',' || move_string[i] == ']'){
          i++;
          break;
        }
        element += move_string[i];
        i++;
      }
      result.push(parseInt(element));
    }
    return result;
  }

  function locate_pieces(){
    for(let n = 1; n < 51; n++){
        let found = false;
        piece_positions.push([]);
        for(let y = 0; y < 8; y++) {
            for(let x = 0; x < 8; x++) {
                if(board[y][x] == n) {
                    piece_positions[n-1].push([]);
                    piece_positions[n-1][0].push(y);
                    piece_positions[n-1][0].push(x);
                    found = true;
                    break;
                }
            }
            if(found){
                break;
            }
        }
        if(!found){
            piece_positions[n-1].push([]);
            piece_positions[n-1][0].push(-1);
            piece_positions[n-1][0].push(-1);
        }
        found = false;
        for(let y = 0; y < 8; y++) {
            for(let x = 0; x < 8; x++) {
                if(board[y][x] == -n) {
                    piece_positions[n-1].push([]);
                    piece_positions[n-1][1].push(y);
                    piece_positions[n-1][1].push(x);
                    found = true;
                    break;
                }
            }
            if(found){
                break;
            }
        }
        if(!found){
            piece_positions[n-1].push([]);
            piece_positions[n-1][1].push(-1);
            piece_positions[n-1][1].push(-1);
        }
    }
  }

  function drawBoard(){
    return new Promise(resolve => {
      ctx.drawImage(boardimg, 0, 0);
      for(let x = 0; x < 8; x++){
        for(let y = 0; y < 8; y++){
          if(board[y][x] > 0 && board[y][x] < 10){
            ctx.drawImage(wpawnimg, (botcolor*(7-x)+Number(botcolor == 0)*x)*75, (botcolor*(7-y)+Number(botcolor == 0)*y)*75);
          }
          if(board[y][x] > 9 && board[y][x] < 20){
            ctx.drawImage(wknightimg, (botcolor*(7-x)+Number(botcolor == 0)*x)*75, (botcolor*(7-y)+Number(botcolor == 0)*y)*75);
          }
          if(board[y][x] > 19 && board[y][x] < 30){
            ctx.drawImage(wbishopimg, (botcolor*(7-x)+Number(botcolor == 0)*x)*75, (botcolor*(7-y)+Number(botcolor == 0)*y)*75);
          }
          if(board[y][x] > 29 && board[y][x] < 40){
            ctx.drawImage(wrookimg, (botcolor*(7-x)+Number(botcolor == 0)*x)*75, (botcolor*(7-y)+Number(botcolor == 0)*y)*75);
          }
          if(board[y][x] > 39 && board[y][x] < 50){
            ctx.drawImage(wqueenimg, (botcolor*(7-x)+Number(botcolor == 0)*x)*75, (botcolor*(7-y)+Number(botcolor == 0)*y)*75);
          }
          if(board[y][x] == 50){
            ctx.drawImage(wkingimg, (botcolor*(7-x)+Number(botcolor == 0)*x)*75, (botcolor*(7-y)+Number(botcolor == 0)*y)*75);
          }
          if(board[y][x] < 0 && board[y][x] > -10){
            ctx.drawImage(bpawnimg, (botcolor*(7-x)+Number(botcolor == 0)*x)*75, (botcolor*(7-y)+Number(botcolor == 0)*y)*75);
          }
          if(board[y][x] < -9 && board[y][x] > -20){
            ctx.drawImage(bknightimg, (botcolor*(7-x)+Number(botcolor == 0)*x)*75, (botcolor*(7-y)+Number(botcolor == 0)*y)*75);
          }
          if(board[y][x] < -19 && board[y][x] > -30){
            ctx.drawImage(bbishopimg, (botcolor*(7-x)+Number(botcolor == 0)*x)*75, (botcolor*(7-y)+Number(botcolor == 0)*y)*75);
          }
          if(board[y][x] < -29 && board[y][x] > -40){
            ctx.drawImage(brookimg, (botcolor*(7-x)+Number(botcolor == 0)*x)*75, (botcolor*(7-y)+Number(botcolor == 0)*y)*75);
          }
          if(board[y][x] < -39 && board[y][x] > -50){
            ctx.drawImage(bqueenimg, (botcolor*(7-x)+Number(botcolor == 0)*x)*75, (botcolor*(7-y)+Number(botcolor == 0)*y)*75);
          }
          if(board[y][x] == -50){
            ctx.drawImage(bkingimg, (botcolor*(7-x)+Number(botcolor == 0)*x)*75, (botcolor*(7-y)+Number(botcolor == 0)*y)*75);
          }
        }
      }
      requestAnimationFrame(() => resolve());
    });
  }

  function update_position(move){
    let n = move[0], y0 = move[1], x0 = move[2], y1 = move[3], x1 = move[4];
    let promoteto;
    if(board[y1][x1] != 0){
      piece_positions[Math.abs(board[y1][x1])-1][Number(n>0)][0] = -1;
    }
    if(Math.abs(n) == 50){
      if(Math.abs(x1-x0) > 1){
        //castle
        let whichrook = Math.sign(n)*(30 + (x1 > 4));
        let rookx = (x1 > 4)*7;
        board[y1][rookx] = 0;
        board[y1][x1 + Math.sign(4-x1)] = whichrook;
        castled *= (2*(n > 0) + 3*(n < 0));
        piece_positions[Math.abs(whichrook)-1][Number(n<0)][0] = y1;
        piece_positions[Math.abs(whichrook)-1][Number(n<0)][1] = x1 + Math.sign(4-x1);
      }
      kingmoved *= ((n>0)*2 + (n<0)*3);
    }
    if(Math.abs(n) == 30 || Math.abs(n) == 31){
      rookmoved[Number(n>0)][Math.abs(n)-30] = 1;
    }
    if((n > 0 && n < 10 && y1 == 7) || (n < 0 && n > -10 && y1 == 0)){
      promoteto = 4;
      board[y1][x1] = Math.sign(n)*(promoteto*10+pieces[promoteto][Number(n < 0)]);
      pieces[promoteto][Number(n < 0)]++;
      piece_positions[Math.abs(board[y1][x1])-1][Number(board[y1][x1]<0)][0] = y1;
      piece_positions[Math.abs(board[y1][x1])-1][Number(board[y1][x1]<0)][1] = x1;
      piece_positions[Math.abs(n)-1][Number(n<0)][0] = -1;
    }else{
      board[y1][x1] = n;
      piece_positions[Math.abs(n)-1][Number(n<0)][0] = y1;
      piece_positions[Math.abs(n)-1][Number(n<0)][1] = x1;
    }
    if(enpassant >= 0 && x1*8+y1 == enpassant && Math.abs(n) < 10){
      piece_positions[Math.abs(board[y1-Math.sign(y1 - y0)][x1])-1][Number(n>0)][0] = -1;
      board[y1-Math.sign(y1 - y0)][x1] = 0;
    }
    if(Math.abs(n) < 10 && Math.abs(y1-y0) > 1){
      enpassant = x1*8+y0+Math.sign(y1 - y0);
    }else{
      enpassant = -1;
    }
    board[y0][x0] = 0;
    positions.push(JSON.parse(JSON.stringify(board)));
  }

  function get_repeated_positions(){
    let repeated_positions = [];
    for(let i = moves%2; i < positions.length; i += 2){
      for(let j = moves%2; j < i; j += 2){
        if(JSON.stringify(positions[i]) == JSON.stringify(positions[j])){
          if(i%2 == 0){
            repeated_positions.push(JSON.parse(JSON.stringify(positions[i])));
          }
          break;
        }
      }
    }
    return repeated_positions;
  }
  
  async function make_move(y0, x0, y1, x1){
    if(!movepiece(y0, x0, y1, x1, turn, JSON.stringify(board), castled
    , JSON.stringify(piece_positions), JSON.stringify(pieces), kingmoved, enpassant, JSON.stringify(rookmoved))){
      console.log("Illegal move");
    }else{
      update_position([board[y0][x0], y0, x0, y1, x1]);
      turn = (turn == 0);
      moves++;
      await drawBoard();
      // wait for next animation frame
      await new Promise(resolve => setTimeout(resolve, 0));
      let repeated_positions = get_repeated_positions();
      let winner = gameend(turn, repeated_positions.length - 1, JSON.stringify(board), JSON.stringify(repeated_positions)
      , JSON.stringify(piece_positions), JSON.stringify(pieces), kingmoved, enpassant, JSON.stringify(rookmoved))
      if(winner >= 0){
        if(winner == 2){
          alert("White won");
        }
        if(winner == 1){
          alert("Black won");
        }
        if(winner == 0){
          alert("Draw");
        }
        turn = -1;
        return;
      }
      let move;
      try{
        move = basicbot(openingbook[0], openingbook[1], repeated_positions.length - 1, JSON.stringify(board), JSON.stringify(repeated_positions)
        , castled, JSON.stringify(piece_positions), JSON.stringify(pieces)
        , kingmoved, enpassant, JSON.stringify(rookmoved));
        console.log(move);
      }catch(error){
        console.log("Error occurred in basicbot:", error);
        alert("Error occured")
        return;
      }
      turn = (turn == 0);
      moves++;
      update_position(convert_move(move));
      await drawBoard();
      // wait for next animation frame
      await new Promise(resolve => setTimeout(resolve, 0));
      repeated_positions = get_repeated_positions();
      winner = gameend(turn, repeated_positions.length - 1, JSON.stringify(board), JSON.stringify(repeated_positions)
      , JSON.stringify(piece_positions), JSON.stringify(pieces), kingmoved, enpassant, JSON.stringify(rookmoved))
      if(winner >= 0){
        if(winner == 2){
          alert("White won");
        }
        if(winner == 1){
          alert("Black won");
        }
        if(winner == 0){
          alert("Draw");
        }
        turn = -1;
        return;
      }
    }
  }

  function select_pieces(e){
    let rect = canvas.getBoundingClientRect();
    if(click == 0){
      x0 = Math.floor((e.clientX - rect.left)/75);
      x0 = (botcolor*(7-x0)+Number(botcolor == 0)*x0);
      y0 = Math.floor((e.clientY - rect.top)/75);
      y0 = (botcolor*(7-y0)+Number(botcolor == 0)*y0);
      if((board[y0][x0] > 0 && botcolor == 1) || (board[y0][x0] < 0 && botcolor == 0)){
        click = 1;
      }
      return;
    }
    if(click == 1){
      x1 = Math.floor((e.clientX - rect.left)/75);
      x1 = (botcolor*(7-x1)+Number(botcolor == 0)*x1);
      y1 = Math.floor((e.clientY - rect.top)/75);
      y1 = (botcolor*(7-y1)+Number(botcolor == 0)*y1);
      make_move(y0, x0, y1, x1);
      click = 0;
    }
  }

  document.addEventListener('mousedown', select_pieces);

  (async () => {
      let binaryData;
      if(botcolor == 0){
        binaryData = await loadBinaryData('openingbook.bin');
      }else{
        binaryData = await loadBinaryData('bopeningbook.bin');
      }

      basicbot = Module.cwrap('basicbot', 'string', ['number', 'number', 'number', 'string', 'string'
      , 'number', 'string', 'string', 'number', 'number', 'string']);

      const dataPtr = Module._malloc(binaryData.length);
      Module.HEAPU8.set(binaryData, dataPtr);

      openingbook = [dataPtr, binaryData.length];

      locate_pieces();
      drawBoard();
  })();
};