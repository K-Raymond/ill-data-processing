# ill-data-processing

The ROOT macros and `TXPConfig` are intended to be used alongside the package of functions provided from the FIPPS collaboration at ILL.

## Steps to success:

1. Convert `.lst` event lists into root TTrees using `Lst2Root.cpp`

2. Calibrate one channel by hand:
..* Uncalibrated spectra are contained under `QA/ADCSpectraNoCal`
..* Create a master `XPConfig.txt`, and copy each coefficent to each channel. `Cal0` is slope, and `Cal1` is offset.

3. Use `EngMatrix.C` to make an energy vs. channel histogram. A `.root` file is automatically exported.

4. Use `LinearGainMatch.C` to make `XPLinear.txt`. The following transition energies can be used depending on the source.

...| Isotope | Energy | Notes |
...| <sup>152</sup>Eu | 121.7817 (3) keV | |
...|                  | 1408.013 (3) keV | |
...| <sup>28</sup>Al  | 1778.987 (15) keV | Beta Decay of <sup>28</sup>Al |
...|                  | 7724.034 (7) keV  | |

...When satisfied with the linear gain match. Save `XPLinear.txt` as `XPConfig.txt` in the `XPConfigs/` folder

5. Fill out the `XPGeometry.txt` LUT located in `XPConfigs/` according to your experiment.

6. Use `GainMatchRoots.C` to gain match each isotope using the above suggested transition energies. Check the energy matricies in each root file containing the source TTrees.

7. Modify `fit_macro/Al-Residual-Fits.C` to better fit your experiment. Then use `AnalyzeResiduals.C` to create the `XPResiduals.txt` file. Examine the `ResidualDiagnostics.root` to inspect the quality of the automatic fits.

8. Examine <sup>152</sup>Eu with the residuals, and check if it makes sense.

9. Gain match experiment, and produce TTree's containing calibrated energies.

10. Analyize!

## Notes:

The software to turn the unordered `.lst` files into sorted root `TTree`'s can be found at,

[lst-data-process-128](https://code.ill.fr/lst/lst-data-process-128)

In particular, the two main files to generate the `TTree`s are `CoincidenceOffline.cpp` and `Lst2Root.cpp`.

TODO: Add more details about creating TTrees using the `lst-data-process` package.

After each `.lst` file has been transformed into a sorted `TTree`, the `.C` macros can be used to produce 1D and 2D analysis histograms. Most macros need to be loaded into root with `.L <macro>.C++`.

All the ROOT macros use `TFileCollection`s to define the input list of `.root` files. File collections can be generated using `find`. For example,

`$ find /data3/illdata/rootfiles/*.root > rootlist.txt`

And then setting the input argument to `rootlist.txt`.

## TODO:

- Create a nice folder structure
- Remove hardcoded text links
- Add lots of documentation to make repo useful
- Make the macros more user friendly and make it easier to do partial analysis with different XPConfigs.
- Add more geometry information and a macro to compute angular correlations
- Create a better gain matching system. It does a good job, but it could be much better!
