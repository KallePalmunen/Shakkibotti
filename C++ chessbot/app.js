// Load the WASM module
Module.onRuntimeInitialized = function () {
  let ctx = document.getElementById("canvas").getContext("2d");

  let move = "", botcolor = 1, turn = 0;

  let boardimg = new Image();
  boardimg.src = "../Images/board.png";
  let wpawnimg = new Image();
  wpawnimg.src = "../Images/whitepawn.png";
  let wknightimg = new Image();
  wknightimg.src = "../Images/whiteknight.png";
  let wbishopimg = new Image();
  wbishopimg.src = "../Images/whitebishop.png";
  let wrookimg = new Image();
  wrookimg.src = "../Images/whiterook.png";
  let wqueenimg = new Image();
  wqueenimg.src = "../Images/whitequeen.png";
  let wkingimg = new Image();
  wkingimg.src = "../Images/whiteking.png";
  let bpawnimg = new Image();
  bpawnimg.src = "../Images/blackpawn.png";
  let bknightimg = new Image();
  bknightimg.src = "../Images/blackknight.png";
  let bbishopimg = new Image();
  bbishopimg.src = "../Images/blackbishop.png";
  let brookimg = new Image();
  brookimg.src = "../Images/blackrook.png";
  let bqueenimg = new Image();
  bqueenimg.src = "../Images/blackqueen.png";
  let bkingimg = new Image();
  bkingimg.src = "../Images/blackking.png";

  // Interact with the C++ chess bot using ccall or cwrap
  const movepiece = Module.cwrap('movepiece', 'number', ['string', 'number']);
  const locate_pieces = Module.cwrap('locate_pieces', 'null', []);
  const add_to_positions = Module.cwrap('add_to_positions', 'null', ['']);
  const set_can_move_positions = Module.cwrap('set_can_move_positions', 'null', ['']);
  const printboard = Module.cwrap('printboard', 'null', ['']);
  const gameend = Module.cwrap('gameend', 'number', ['number']);
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
  }
  
  function keypress(event) {
    if (event.keyCode === 13) {
      console.log(turn);
      move = prompt("move");
      if(!movepiece(move, turn)){
        console.log("Illegal move");
      }else{
        turn = (turn == 0);
        drawBoard();
        set_can_move_positions();
        if(gameend(turn) >= 0){
          console.log(gameend(turn))
          turn = -1;
        }
        console.log(basicbot(openingbook[0], openingbook[1]));
        turn = (turn == 0);
        drawBoard();
        set_can_move_positions();
        if(gameend(turn) >= 0){
          console.log(gameend(turn))
          turn = -1;
        }
      }
    }
  }

  document.addEventListener('keydown', keypress);

  (async () => {
      let binaryData;
      if(botcolor == 0){
        binaryData = await loadBinaryData('openingbook.bin');
      }else{
        binaryData = await loadBinaryData('bopeningbook.bin');
      }

      basicbot = Module.cwrap('basicbot', 'string', ['number', 'number']);

      const dataPtr = Module._malloc(binaryData.length);
      Module.HEAPU8.set(binaryData, dataPtr);

      openingbook = [dataPtr, binaryData.length];

      console.log("hi");
      locate_pieces();
      add_to_positions();
      set_can_move_positions();
      printboard();
      drawBoard();
  })();
};