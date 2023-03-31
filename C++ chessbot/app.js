// Load the WASM module
Module.onRuntimeInitialized = function () {
    // Interact with the C++ chess bot using ccall or cwrap
    const SumFunction = Module.cwrap('sum', 'number', ['number', 'number']);
    const movepiece = Module.cwrap('movepiece', 'number', ['string']);
    const basicbot = Module.cwrap('basicbot', '', ['string']);
    
    // Call the wrapped function
    const result = SumFunction(4,3);
    // Update the UI based on the result
    console.log("hi");
    console.log(result);
    console.log(basicbot());
    console.log(movepiece("e7e5"));
    console.log(movepiece("e2e4"));
  };