# ill-data-processing

The ROOT macros and the experimental configuration class are intended to be used alongside the package of functions provided with the FIPPS array at ILL.
The software to turn the unordered `.lst` files into sorted root `TTree`'s can be found at,

[lst-data-process-128](https://code.ill.fr/lst/lst-data-process-128)

In particular, the two main files to generate the `TTree`s are `CoincidenceOffline.cpp` and `Lst2Root.cpp`.

TODO: Add more details about creating TTrees using the `lst-data-process` package.

After each `.lst` file has been transformed into a sorted `TTree`, the `.C` macros can be used to produce 1D and 2D analysis histograms. Most macros need to be loaded into root with `.L <macro>.C++`.

Steps to success:

1. Find a singles spectra from one of the generated `.root` files. These can be found in the `QA` subfolder. Manually fit calibration peaks in one channel, and manually calculate the basic energy coefficents. Input the slope (cal0) and intercept (cal1) into every row in XPConfig.txt. Add the geometry information into `XPGeometry.txt`.

2. Produce calibration histograms using functions in `ConstructEng.C`. Use `LinearGainMatch.C` interactively (`.L LinearGainMatch.C`) to roughly gain match all channels. This spits out `XPLinear.txt`.

    1. Initially set the width1 and width2 to a wide range ~100 keV for the first process
    2. As the calibration improves after successive gain matches, the bounds can be made smaller and be used to produce a good linear calibration.

3. Generate the energy spectra using all usefull data and verify quality of the set as a whole. Then generate the analysis spectra with `ConstructGG.C`.

All the ROOT macros use `TFileCollection`s to define the input list of `.root` files. File collections can be generated using `find`. For example,

`$ find /data3/illdata/rootfiles/*.root > rootlist.txt`

And then setting the input argument to `rootlist.txt`.

TODO:

- Create a nice folder structure
- Remove hardcoded text links
- Add lots of documentation to make repo useful
- Make the macros more user friendly and make it easier to do partial analysis with different XPConfigs.
- Add more geometry information and a macro to compute angular correlations
- Create a better gain matching system. It does a good job, but it could be much better!
