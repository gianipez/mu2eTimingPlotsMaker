// Runs all timing plots
// Usage: root -l -b -q 'runAllTiming.C("input_dir/your_csv_dir/csv_XBB/")'

// runAllTiming.C
// Usage: root -l -b -q runAllTiming.C'("input_dir/your_dir_with_csv/csv_XBB/")'

#include "./PlotTimes.C"       
#include "./doTimingPlots.C"   

void runAllTiming(const char* inputDir = "input_dir/test/csv_2BB/") {
  gStyle->SetOptStat(111110);
  gStyle->SetOptTitle(0);

  plotAllTiming(inputDir);
}
