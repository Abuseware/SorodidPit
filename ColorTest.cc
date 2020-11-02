/*
 * Copyright (c) 2018, Artur Kaleta
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

const unsigned char STEP = 1;
const unsigned char FPS = 120;

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "ColorTest", sf::Style::Fullscreen);
    window.setFramerateLimit(FPS);
    window.setMouseCursorVisible(false);

    enum State { BlackToRed, RedToGreen, GreenToBlue, BlueToWhite, WhiteToBlack, Exit};

    sf::Uint8 r = 0, g = 0, b = 0;
    State state = BlackToRed;
    bool pause = false;

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event)) {

            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                        pause = true; // Skip unnecessary redraws
                    }
                    if (event.key.code == sf::Keyboard::Space) pause = !pause;
                    break;
                case sf::Event::MouseButtonPressed:
                    if(event.mouseButton.button == sf::Mouse::Left) pause = false;
                default:
                    break;
            }
        }

        if(!pause){
            switch(state) {
                case BlackToRed:
                    r += STEP;
                    if (r == 255) {
                        state = RedToGreen;
                        pause = true;
                    }
                    break;

                case RedToGreen:
                    g += STEP;
                    r -= STEP;
                    if (g == 255) {
                        state = GreenToBlue;
                        pause = true;
                    }
                    break;

                case GreenToBlue:
                    b += STEP;
                    g -= STEP;
                    if (b == 255){
                        state = BlueToWhite;
                        pause = true;
                    }
                    break;

                case BlueToWhite:
                    r += STEP;
                    g = r;
                    if (r == 255) {
                        state = WhiteToBlack;
                        pause = true;
                    }
                    break;

                case WhiteToBlack:
                    r -= STEP;
                    g = b = r;
                    if (r == 0) {
                        state = Exit;
                        pause = true;
                    }
                    break;
                case Exit:
                    window.close();
                    pause = true;
                    break;
            }

            if(window.isOpen()){
                window.clear(sf::Color(r, g, b));
                window.display();
            }
        }

    }

    return 0;
}
