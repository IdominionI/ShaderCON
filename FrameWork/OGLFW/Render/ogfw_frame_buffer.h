#pragma once

//#define GLEW_STATIC // Must have this present or will have message for a glew.dll file
//#include <GL/glew.h>



class oglfw_frame_buffer_class {
public:
	oglfw_frame_buffer_class() {}
	~oglfw_frame_buffer_class() { delete_frame_buffer(); }


	GLuint texture_id = 0;

	// here we create our framebuffer and our renderbuffer
	// you can find a more detailed explanation of framebuffer 
	// on the official opengl homepage, see the link above
	void create_framebuffer( GLint WIDTH, GLint HEIGHT)
	{
		//if (texture_id == 0) {
		//	printf("ERROR::FRAMEBUFFER:: Framebuffer texture not defined !\n");
		//	return;
		//}

		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("ERROR::FRAMEBUFFER:: create_framebuffer : Framebuffer is not complete!\n");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void delete_frame_buffer() {
		glDeleteFramebuffers(1, &FBO);
		glDeleteTextures(1, &texture_id);
		glDeleteRenderbuffers(1, &RBO);
	}

	// here we bind our framebuffer
	void bind_framebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	}

	// here we unbind our framebuffer
	void unbind_framebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// and we rescale the buffer, so we're able to resize the window
	void rescale_framebuffer(float width, float height)	{
//printf("FRAMEBUFFER::rescale_framebuffer Framebuffer texture not defined  %f : %f : %i!\n", width, height, texture_id);
		if (texture_id == 0) {
			printf("ERROR::FRAMEBUFFER::rescale_framebuffer Framebuffer texture not defined  %i !\n", texture_id);
			return;
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	}

	GLuint FBO;
	GLuint RBO;

private:

};