This is the main repository for IHU Android

To download the complete repo structure including aosp, which is required to build etc:

   repo init -u https://icup_android.gerrit.cm.volvocars.biz/a/manifest.git
   repo sync --no-clone-bundle


To build the complete system:

   vendor/volvocars/tools/docker_build/run.sh "lunch ihu_vcc-eng && make -j8"


See also: https://c1.confluence.cm.volvocars.biz/display/IHUA/06+Development 



