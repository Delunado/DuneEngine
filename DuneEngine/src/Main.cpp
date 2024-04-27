#include "Application.h"

int main(int argc, char* argv[])
{
    Application app;

    app.Setup();

    while (app.IsRunning())
    {
        app.Input();
        app.Update();
        app.FixedUpdate();
        app.Render();
    }

    app.Cleanup();

    return 0;
}
