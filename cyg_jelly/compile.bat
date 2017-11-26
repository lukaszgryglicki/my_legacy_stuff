del *.o jelly.exe
g++ -O3 -c bern_base.cxx
g++ -O3 -c bez_patch.cxx
g++ -O3 -c bez_wall.cxx
g++ -O3 -c canvas.cxx
g++ -O3 -c frame.cxx
g++ -O3 -c main.cxx
g++ -O3 -c mass.cxx
g++ -O3 -c room.cxx
g++ -O3 -c scene.cxx
g++ -O3 -c tri.cxx
g++ -O3 -c suszi.cxx
g++ -O3 -o jelly.exe bern_base.o bez_patch.o bez_wall.o canvas.o frame.o main.o mass.o room.o scene.o tri.o suszi.o -lglut32 -lopengl32 -lglu32
