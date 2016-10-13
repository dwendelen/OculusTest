#pragma once

namespace video {
    class Displayy
    {
    public:
        Displayy() {};
        virtual void swap() {};
        virtual void prepareForNewFrame() {};
        virtual int getWidth() = 0;
        virtual int getHeight() = 0;
        virtual ~Displayy() {};
    };
}
