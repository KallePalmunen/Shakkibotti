// Load the WASM module
Module.onRuntimeInitialized = function () {
  let ctx = document.getElementById("canvas").getContext("2d");
  let boardimg = new Image();
  boardimg.src = "../Images/board.png";

  // Interact with the C++ chess bot using ccall or cwrap
  const movepiece = Module.cwrap('movepiece', 'number', ['string']);
  const basicbot = Module.cwrap('basicbot', 'null', ['string']);
  const locate_pieces = Module.cwrap('locate_pieces', 'null', []);
  const add_to_positions = Module.cwrap('add_to_positions', 'null', ['']);
  const set_can_move_positions = Module.cwrap('set_can_move_positions', 'null', ['']);
  const printboard = Module.cwrap('printboard', 'null', ['']);
  const gameend = Module.cwrap('gameend', 'null', ['']);

  function drawBoard(){
    ctx.drawImage(boardimg, 0, 0);
  }

  console.log("hi");
  locate_pieces();
  add_to_positions();
  set_can_move_positions();
  printboard();
  drawBoard();

  while(true){
    move = prompt("move");
    while(!movepiece(move)){
      console.log("Illegal move");
      move = prompt("move");
    }
    set_can_move_positions();
    gameend();
    console.log(basicbot());
    set_can_move_positions();
    gameend();
  }
};