HELPERS=src/Camera.cpp src/IndexBuffer.cpp src/VertexBuffer.cpp src/Shader.cpp src/Texture.cpp
FLAT=src/main_flat.cpp
SPHERE=src/main_sphere.cpp

hw3_flat:
	g++ $(FLAT) $(HELPERS) -o hw3_flat -std=c++11 -lXi -lGLEW -lGLU -lm -lGL -lm -lpthread -ldl -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -ljpeg

hw3_sphere:
	g++ $(SPHERE) $(HELPERS) -o hw3_sphere -std=c++11 -lXi -lGLEW -lGLU -lm -lGL -lm -lpthread -ldl -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -ljpeg

clean:
	rm -f hw3_flat hw3_sphere
