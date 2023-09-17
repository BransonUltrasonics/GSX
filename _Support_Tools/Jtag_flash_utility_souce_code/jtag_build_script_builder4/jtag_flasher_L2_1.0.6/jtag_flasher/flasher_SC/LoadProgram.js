// Import the DSS packages into our namespace to save on typing
importPackage(Packages.com.ti.debug.engine.scripting)
importPackage(Packages.com.ti.ccstudio.scripting.environment)
importPackage(Packages.java.lang)

// Configurable Parameters
var deviceCCXMLFile = "sc_afterbootup.ccxml";
var programToLoad = "qspi_idkAM572x_armv7.out";

// Create our scripting environment object - which is the main entry point into any script and
// the factory for creating other Scriptable ervers and Sessions
var script = ScriptingEnvironment.instance();

// Create a debug server
var ds = script.getServer( "DebugServer.1" );

// Set the device ccxml 
ds.setConfig( deviceCCXMLFile );

// Open a debug session
debugSession = ds.openSession( "Texas Instruments XDS2xx USB Onboard Debug Probe_0/CortexA15_0" );
debugSession.target.connect();
script.traceWrite("program loading.....");
// Load the program
try
{
	debugSession.target.reset();
	debugSession.memory.loadProgram( programToLoad );
	script.traceWrite("program loaded successfully");
}
catch(ex)
{
	script.traceWrite(programToLoad + " does not exist! Aborting script");
}
// Run the program
//debugSession.target.run();
//var address = debugSession.symbol.getAddress("main")
//debugSession.memory.writeRegister("PC", address);
debugSession.target.runAsynch();
debugSession.target.disconnect();
debugSession.terminate();
ds.stop();
script.traceWrite("program flashing...........");
