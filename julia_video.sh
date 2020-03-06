image_import_folder="temp_imgs"
video_export_folder="temp_vids"
fps=25

echo "Setting up the image folder ..."
mkdir -p $image_import_folder &&
rm -f $image_import_folder/*.ppm &&
echo "Compiling the code ..." &&
g++ julia.cpp lib/julia_polynomial_series.cpp lib/color_mixer.cpp -o julia &&
echo "Creating ppm files ..." &&
./julia.exe &&
rm -f julia.exe &&
echo "Making video ..." &&
python ppms_to_avi.py $image_import_folder $video_export_folder"/julia_series.avi" $fps &&
echo "Finished video"