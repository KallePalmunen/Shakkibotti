// Load the WASM module
Module.onRuntimeInitialized = function () {
    // Interact with the C++ chess bot using ccall or cwrap
    const intsign = Module.cwrap('intsign', 'number', ['number']);
    
    // Call the wrapped function
    const result = intsign(10);
    // Update the UI based on the result
    console.log("hi");
    console.log(result);
  };