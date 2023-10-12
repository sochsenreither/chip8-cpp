#pragma once

class Engine {
   private:
    void update();
    void draw();

   public:
    void load_rom();
    void start();
};