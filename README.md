# Getting Started
1) Create particle account https://build.particle.io/build/new (you should get an invite to the existing account)
2) Registering argon via iPhone or Android app (follow on screen instructions)
3) View device at https://build.particle.io/build/new 
4) Download VS Code https://code.visualstudio.com/?wt.mc_id=vscom_downloads
5) In VS Code View >> Extentions (Mac) search for particle workbench 
6) Install Particle Workbench extention in VS Code
7) Check if particle-cli is installed in a terminal window `particle --version`
8) Update devices firmware
   1) Press and hold both the RESET/RST and MODE/SETUP buttons simultaneously.
   2) Release only the RESET/RST button while continuing to hold the MODE/SETUP button.
   3) Release the MODE/SETUP button once the device begins to `blink yellow`.
   4) In a terminal window `particle update`
9) Select device from the bottom bar in VS Code `xxxxxxxxxxxxxxxxxxxxxxx`
   1)  Should say argon and deviceOS@1.5.2 in addtion to device ID
10) Connect device to WIFI (note it is connected when breathing light blue / cyan)
    1)  Run `particle serial wifi` in terminal and follow the instructions
11) Select Flash at the top right 
12) After successfully flashing in a terminal window type `particle serial monitor`
13) Run the seismic-server in another VS Code window `npm start` 