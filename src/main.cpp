#include "engine.h"

int main(){
    Engine* engine = new Engine(1080, 720, "Frost Engine");
    engine->Loop();

    delete(engine);
}