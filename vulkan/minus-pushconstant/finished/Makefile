all: engine


BACKEND = src/backend/file.o \
	  src/backend/glfw_backend.o

RENDERER = src/renderer/device.o \
	   src/renderer/frame.o \
	   src/renderer/image.o \
	   src/renderer/instance.o \
	   src/renderer/renderer.o \
	   src/renderer/shader.o \
	   src/renderer/swapchain.o \
           src/renderer/command.o \
	   src/renderer/synchronisation.o \
           src/renderer/pushconstants.o

CONTROLLER = src/controller/app.o

LOGGING = src/logging/logger.o

engine: src/main.o $(BACKEND) $(LOGGING) $(CONTROLLER) $(RENDERER)
	clang++ -std=c++20 -DVULKAN_HPP_NO_EXCEPTIONS -DGLFW_INCLUDE_VULKAN -o $@ $^ -lvulkan -lglfw -lshaderc

%.o: %.cpp
	clang++ -std=c++20 -c -DVULKAN_HPP_NO_EXCEPTIONS -DGLFW_INCLUDE_VULKAN $< -o $@ 

#shaders: shaders/vertex.spv shaders/fragment.spv

#shaders/vertex.spv: shaders/shader.vert
#	glslang --target-env vulkan1.3 -gVS -V $< -o $@

#shaders/fragment.spv: shaders/shader.frag
#	glslang --target-env vulkan1.3 -gVS -V $< -o $@
