CXXFLAGS= -I/usr/X11R6/include -O3 
LIBS= -L/usr/X11R6/lib -lglut -pthread -O3
CXX=g++
STRIP= strip -s
all: jelly
jelly: bern_base.o bez_patch.o bez_wall.o frame.o room.o tri.o mass.o scene.o canvas.o suszi.o main.o
	$(CXX) -o jelly bern_base.o bez_patch.o bez_wall.o canvas.o main.o frame.o mass.o suszi.o scene.o room.o tri.o $(LIBS)
	$(STRIP) jelly
bern_base.o: bern_base.cxx bern_base.hxx defs.hxx
	$(CXX) $(CXXFLAGS) -c bern_base.cxx
bez_patch.o: bez_patch.cxx bez_patch.hxx defs.hxx vec3d.hxx bern_tens_base.hxx bern_base.hxx
	$(CXX) $(CXXFLAGS) -c bez_patch.cxx
bez_wall.o: bez_wall.cxx bez_wall.hxx defs.hxx obj3d.hxx bez_patch.hxx
	$(CXX) $(CXXFLAGS) -c bez_wall.cxx
frame.o: frame.cxx frame.hxx defs.hxx obj3d.hxx spring.hxx
	$(CXX) $(CXXFLAGS) -c frame.cxx
room.o: room.cxx room.hxx defs.hxx vec3d.hxx obj3d.hxx
	$(CXX) $(CXXFLAGS) -c room.cxx
tri.o: tri.cxx tri.hxx defs.hxx vec3d.hxx obj3d.hxx
	$(CXX) $(CXXFLAGS) -c tri.cxx
mass.o: mass.cxx mass.hxx defs.hxx vec3d.hxx obj3d.hxx frame.hxx spring.hxx room.hxx
	$(CXX) $(CXXFLAGS) -c mass.cxx
scene.o: scene.cxx scene.hxx defs.hxx vec3d.hxx obj3d.hxx bez_wall.hxx frame.hxx mass.hxx matrix4d.hxx room.hxx spring.hxx suszi.hxx
	$(CXX) $(CXXFLAGS) -c scene.cxx
canvas.o: canvas.cxx canvas.hxx defs.hxx scene.hxx matrix4d.hxx
	$(CXX) $(CXXFLAGS) -c canvas.cxx
suszi.o: suszi.cxx suszi.hxx defs.hxx bern_tri_base.hxx scene.hxx tri.hxx point3d.hxx obj3d.hxx matrix4d.hxx
	$(CXX)  $(CXXFLAGS) -c suszi.cxx
main.o: main.cxx defs.hxx canvas.hxx
	$(CXX) $(CXXFLAGS) -c main.cxx
clean:
	-rm *.o jelly
