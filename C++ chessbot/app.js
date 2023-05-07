// Load the WASM module
Module.onRuntimeInitialized = function () {
  let ctx = document.getElementById("canvas").getContext("2d");

  let move = "", botcolor = 1, turn = 0, click = 0, x0, y0, x1, y1, moves = 0, 
  board = [[30,10,20,50,40,21,11,31],[1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
  [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[-30,-10,-20,-50,-40,-21,-11,-31]], 
  positions = [[[30,10,20,50,40,21,11,31],[1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
  [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[-30,-10,-20,-50,-40,-21,-11,-31]]];

  let boardimg = new Image();
  boardimg.src = "../Images/board.png";
  let wpawnimg = new Image();
  wpawnimg.src = "../Images/whitepawn2.png";
  let wknightimg = new Image();
  wknightimg.src = "../Images/whiteknight2.png";
  let wbishopimg = new Image();
  wbishopimg.src = "../Images/whitebishop2.png";
  let wrookimg = new Image();
  wrookimg.src = "../Images/whiterook2.png";
  let wqueenimg = new Image();
  wqueenimg.src = "../Images/whitequeen2.png";
  let wkingimg = new Image();
  wkingimg.src = "../Images/whiteking2.png";
  let bpawnimg = new Image();
  bpawnimg.src = "../Images/blackpawn2.png";
  let bknightimg = new Image();
  bknightimg.src = "../Images/blackknight2.png";
  let bbishopimg = new Image();
  bbishopimg.src = "../Images/blackbishop2.png";
  let brookimg = new Image();
  brookimg.src = "../Images/blackrook2.png";
  let bqueenimg = new Image();
  bqueenimg.src = "../Images/blackqueen2.png";
  let bkingimg = new Image();
  bkingimg.src = "../Images/blackking2.png";

  // Interact with the C++ chess bot using ccall or cwrap
  const movepiece = Module.cwrap('movepiece', 'number', ['number', 'number', 'number', 'number', 'number']);
  const locate_pieces = Module.cwrap('locate_pieces', 'null', []);
  const add_to_positions = Module.cwrap('add_to_positions', 'null', ['']);
  const set_can_move_positions = Module.cwrap('set_can_move_positions', 'null', ['']);
  const printboard = Module.cwrap('printboard', 'null', ['']);
  const gameend = Module.cwrap('gameend', 'number', ['number', 'number']);
  const get_board = Module.cwrap('get_board_element', 'number', ['number, number']);
  let basicbot;

  let openingbook;

  async function loadBinaryData(url) {
    const response = await fetch(url);
    const buffer = await response.arrayBuffer();
    const data = new Uint8Array(buffer);
    return data;
  }

  function drawBoard(){
    return new Promise(resolve => {
      ctx.drawImage(boardimg, 0, 0);
      for(let x = 0; x < 8; x++){
        for(let y = 0; y < 8; y++){
          if(get_board(y,x) > 0 && get_board(y,x) < 10){
            ctx.drawImage(wpawnimg, x*75, y*75);
          }
          if(get_board(y,x) > 9 && get_board(y,x) < 20){
            ctx.drawImage(wknightimg, x*75, y*75);
          }
          if(get_board(y,x) > 19 && get_board(y,x) < 30){
            ctx.drawImage(wbishopimg, x*75, y*75);
          }
          if(get_board(y,x) > 29 && get_board(y,x) < 40){
            ctx.drawImage(wrookimg, x*75, y*75);
          }
          if(get_board(y,x) > 39 && get_board(y,x) < 50){
            ctx.drawImage(wqueenimg, x*75, y*75);
          }
          if(get_board(y,x) == 50){
            ctx.drawImage(wkingimg, x*75, y*75);
          }
          if(get_board(y,x) < 0 && get_board(y,x) > -10){
            ctx.drawImage(bpawnimg, x*75, y*75);
          }
          if(get_board(y,x) < -9 && get_board(y,x) > -20){
            ctx.drawImage(bknightimg, x*75, y*75);
          }
          if(get_board(y,x) < -19 && get_board(y,x) > -30){
            ctx.drawImage(bbishopimg, x*75, y*75);
          }
          if(get_board(y,x) < -29 && get_board(y,x) > -40){
            ctx.drawImage(brookimg, x*75, y*75);
          }
          if(get_board(y,x) < -39 && get_board(y,x) > -50){
            ctx.drawImage(bqueenimg, x*75, y*75);
          }
          if(get_board(y,x) == -50){
            ctx.drawImage(bkingimg, x*75, y*75);
          }
        }
      }
      requestAnimationFrame(() => resolve());
    });
  }

  function update_position(){
    for(let y = 0; y < 8; y++){
      for(let x = 0; x < 8; x++){
        board[y][x] = get_board(y,x)
      }
    }
    positions.push(JSON.parse(JSON.stringify(board)));
    console.log(positions);
  }
  
  async function make_move(y0, x0, y1, x1){
    if(!movepiece(y0, x0, y1, x1, turn)){
      console.log("Illegal move");
    }else{
      update_position();
      turn = (turn == 0);
      moves++;
      await drawBoard();
      set_can_move_positions();
      if(gameend(turn, moves) >= 0){
        console.log(gameend(turn, moves))
        turn = -1;
      }
      // wait for next animation frame
      await new Promise(resolve => setTimeout(resolve, 0));
      basicbot(openingbook[0], openingbook[1], moves);
      turn = (turn == 0);
      moves++;
      drawBoard();
      set_can_move_positions();
      if(gameend(turn, moves) >= 0){
        console.log(gameend(turn, moves))
        turn = -1;
      }
      update_position();
    }
  }

  function select_pieces(e){
    let rect = canvas.getBoundingClientRect();
    if(click == 0){
      x0 = Math.floor((e.clientX - rect.left)/75);
      y0 = Math.floor((e.clientY - rect.top)/75);
      if((get_board(y0, x0) > 0 && botcolor == 1) || (get_board(y0, x0) < 0 && botcolor == 0)){
        click = 1;
      }
      return;
    }
    if(click == 1){
      x1 = Math.floor((e.clientX - rect.left)/75);
      y1 = Math.floor((e.clientY - rect.top)/75);
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

      basicbot = Module.cwrap('basicbot', 'number', ['number', 'number', 'number']);

      const dataPtr = Module._malloc(binaryData.length);
      Module.HEAPU8.set(binaryData, dataPtr);

      openingbook = [dataPtr, binaryData.length];

      locate_pieces();
      //add_to_positions();
      set_can_move_positions();
      drawBoard();
  })();
};