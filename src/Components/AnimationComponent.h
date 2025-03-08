#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

struct AnimationComponent {
    int numFrames;
    int currentFrame;
    int frameSpeedRate;
    bool loop;
    int startTime;

    explicit AnimationComponent(const int numFrames = 1, const int frameSpeedRate = 1,
                                const bool loop = true): numFrames(numFrames),
                                                         currentFrame(1),
                                                         frameSpeedRate(frameSpeedRate),
                                                         loop(loop),
                                                         startTime(SDL_GetTicks()) {
    };
};

#endif //ANIMATIONCOMPONENT_H
