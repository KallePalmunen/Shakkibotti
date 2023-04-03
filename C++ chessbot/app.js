// Load the WASM module
Module.onRuntimeInitialized = function () {
  let ctx = document.getElementById("canvas").getContext("2d");

  let move = "";

  let boardimg = new Image();
  boardimg.src = "../Images/board.png";
  let wpawnimg = new Image();
  wpawnimg.src = "../Images/whitepawn.png";

  // Interact with the C++ chess bot using ccall or cwrap
  const movepiece = Module.cwrap('movepiece', 'number', ['string']);
  const basicbot = Module.cwrap('basicbot', 'null', ['string']);
  const locate_pieces = Module.cwrap('locate_pieces', 'null', []);
  const add_to_positions = Module.cwrap('add_to_positions', 'null', ['']);
  const set_can_move_positions = Module.cwrap('set_can_move_positions', 'null', ['']);
  const printboard = Module.cwrap('printboard', 'null', ['']);
  const gameend = Module.cwrap('gameend', 'null', ['']);
  const get_board = Module.cwrap('get_board_element', 'number', ['number, number']);

  function drawBoard(){
    ctx.drawImage(boardimg, 0, 0);
    for(let x = 0; x < 8; x++){
      for(let y = 0; y < 8; y++){
        if(get_board(y,x) != 0){
          ctx.drawImage(wpawnimg, x*75, y*75);
        }
      }
    }
  }
  
  function keypress(event) {
    if (event.keyCode === 13) {
      move = prompt("move");
      if(!movepiece(move)){
        console.log("Illegal move");
      }
      drawBoard();
      set_can_move_positions();
      gameend();
      console.log(basicbot());
      drawBoard();
      set_can_move_positions();
      gameend();
    }
  }

  // Attach the event listener to the document
  document.addEventListener('keydown', keypress);

  console.log("hi");
  locate_pieces();
  add_to_positions();
  set_can_move_positions();
  printboard();
  drawBoard();
};