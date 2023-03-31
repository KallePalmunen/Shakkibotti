// Load the WASM module
Module.onRuntimeInitialized = function () {
    let ctx = document.getElementById("canvas").getContext("2d");
    let boardimg = new Image();
    boardimg.src = "../Images/board.png"
    // Interact with the C++ chess bot using ccall or cwrap
    const SumFunction = Module.cwrap('sum', 'number', ['number', 'number']);
    const movepiece = Module.cwrap('movepiece', 'number', ['string']);
    const basicbot = Module.cwrap('basicbot', '', ['string']);

    function drawBoard(){
      ctx.drawImage(boardimg, 0, 0);
    }
    
    // Call the wrapped function
    const result = SumFunction(4,3);
    // Update the UI based on the result
    console.log("hi");
    console.log(result);
    console.log(basicbot());
    console.log(movepiece("e7e5"));
    console.log(movepiece("e2e4"));
    drawBoard();
  };