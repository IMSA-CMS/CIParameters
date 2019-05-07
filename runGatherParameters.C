//run gatherParameters.C

{
  //things we've made/updated
  gSystem->CompileMacro("lambdaOverlayedMasses.C","k0");
  gSystem->CompileMacro("lambdaVEvents.C","k0");
  gSystem->CompileMacro("writeToFile.C","k0");
  gSystem->CompileMacro("plotMassCutsReturnHist.C","k0");
  gSystem->CompileMacro("crossSectionMap.C","k0");
  gSystem->CompileMacro("FileParams.C","k0");
  //gSystem->CompileMacro("drawFileParams.C","k0");
  //gSystem->CompileMacro("drawPlotCopy.C","k0");

  //things from scripts folder
  //gSystem->CompileMacro("/uscms/home/pdong/work/IMSACMS/scripts/FileParams.C","k0");
  gSystem->CompileMacro("drawPlotCopy.C","k0");
  gSystem->CompileMacro("drawFileParams.C","k0");
  gSystem->CompileMacro("crossSection.C","k0");

  //the file
  gSystem->CompileMacro("gatherParameters.C","k0");
  gatherParameters();
}
