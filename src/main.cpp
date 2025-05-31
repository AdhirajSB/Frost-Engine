#include "engine.h"

int main(){
    Engine* engine = new Engine("Frost Engine");
    engine->Loop();

    delete(engine);
}