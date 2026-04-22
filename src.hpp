#ifndef T3_ACMOJ2455_TETRIS_HPP
#define T3_ACMOJ2455_TETRIS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cctype>

namespace sjtu {

    class XorShift {
    private:
        unsigned int x, y, z, w;
        void xor_shift32() {
            unsigned int t = x ^ (x << 11);
            x = y;
            y = z;
            z = w;
            w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        }

    public:
        explicit XorShift(unsigned int seed) {
            x = y = z = w = seed;
            xor_shift32();
        }
        unsigned int next() { xor_shift32(); return w; }
    };

    struct Tetris {
        struct Tetromino {
            struct Shape {
                std::vector<std::vector<int>> shape;
                int rotateX, rotateY; // delta applied to top-left when rotating cw to next state
            };
            constexpr static char shapeAlphas[7] = {'I', 'T', 'O', 'S', 'Z', 'J', 'L'};
            const static Shape rotateShapes[7][4];
            int x = 0, y = 0; // top-left of bounding box
            int index = 0;    // which tetromino type
            int rot = 0;      // current rotation 0..3
        };

        const static int WIDTH = 10;
        const static int HEIGHT = 20;

        XorShift rand;
        Tetromino cur;
        std::vector<std::vector<int>> field; // HEIGHT x WIDTH, 0/1
        int score = 0;
        bool has_active = false;

        Tetris(int seed);
        bool new_tetris();
        bool dropping();
        bool move_tetris(const std::string &operation);
        void place_tetris();
        void clear_lines();
        void print_field();
        bool game_over();
        bool check_collision(const Tetromino &next);
        void run();

        // utilities
        void overlay_to(std::vector<std::vector<int>>& out) const;
    };

    const Tetris::Tetromino::Shape Tetris::Tetromino::rotateShapes[7][4] = {
            {
                    {{{1, 1, 1, 1}},         1, -1},
                    {{{1},    {1},    {1}, {1}}, -1, 2},
                    {{{1, 1, 1, 1}},         2, -2},
                    {{{1},    {1},    {1}, {1}}, -2, 1},
            }, // I
            {
                    {{{1, 1, 1}, {0, 1, 0}}, 0, -1},
                    {{{0, 1}, {1, 1}, {0, 1}},   0, 0},
                    {{{0, 1, 0}, {1, 1, 1}}, 1, 0},
                    {{{1, 0}, {1, 1}, {1, 0}},   -1, 1},
            }, // T
            {
                    {{{1, 1},    {1, 1}},    0, 0},
                    {{{1, 1},    {1, 1}},    0, 0},
                    {{{1, 1},    {1, 1}},    0, 0},
                    {{{1, 1},    {1, 1}},    0, 0}
            }, // O
            {
                    {{{0, 1, 1}, {1, 1, 0}}, 0, -1},
                    {{{1, 0}, {1, 1}, {0, 1}},   0, 0},
                    {{{0, 1, 1}, {1, 1, 0}}, 1, 0},
                    {{{1, 0}, {1, 1}, {0, 1}},   -1, 1}
            }, // S
            {
                    {{{1, 1, 0}, {0, 1, 1}}, 0, -1},
                    {{{0, 1}, {1, 1}, {1, 0}},   0, 0},
                    {{{1, 1, 0}, {0, 1, 1}}, 1, 0},
                    {{{0, 1}, {1, 1}, {1, 0}},   -1, 1}
            }, // Z
            {
                    {{{1, 0, 0}, {1, 1, 1}}, 1, 0},
                    {{{1, 1},    {1, 0}, {1, 0}}, -1, 1},
                    {{{1, 1, 1}, {0, 0, 1}}, 0, -1},
                    {{{0, 1},    {0, 1}, {1, 1}},   0, 0}
            }, // J
            {
                    {{{0, 0, 1}, {1, 1, 1}}, 1, 0},
                    {{{1, 0},    {1, 0}, {1, 1}},   -1, 1},
                    {{{1, 1, 1}, {1, 0, 0}}, 0, -1},
                    {{{1, 1},    {0, 1}, {0, 1}},   0, 0}
            } // L
    };

    Tetris::Tetris(int seed) : rand(seed) {
        field.assign(HEIGHT, std::vector<int>(WIDTH, 0));
        has_active = false;
    }

    bool Tetris::check_collision(const Tetromino &next) {
        const auto &mat = Tetromino::rotateShapes[next.index][next.rot].shape;
        int h = (int)mat.size();
        int w = (int)mat[0].size();
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if (mat[i][j]) {
                    int yy = next.y + i;
                    int xx = next.x + j;
                    if (yy < 0 || yy >= HEIGHT || xx < 0 || xx >= WIDTH) return true;
                    if (field[yy][xx]) return true;
                }
            }
        }
        return false;
    }

    bool Tetris::new_tetris() {
        cur.index = rand.next() % 7;
        cur.rot = 0;
        int w = (int)Tetromino::rotateShapes[cur.index][0].shape[0].size();
        cur.x = (WIDTH >> 1) - (w >> 1);
        cur.y = 0;
        has_active = true;
        std::cout << Tetromino::shapeAlphas[cur.index] << '\n';
        if (check_collision(cur)) return false; // condition (b)
        return true;
    }

    bool Tetris::dropping() {
        Tetromino nxt = cur;
        nxt.y += 1;
        if (!check_collision(nxt)) { cur = nxt; return true; }
        return false;
    }

    bool Tetris::move_tetris(const std::string &operation) {
        if (!has_active) return false;
        if (operation == "e") return true;
        if (operation == "space") {
            Tetromino nxt = cur;
            while (true) {
                Tetromino test = nxt;
                test.y += 1;
                if (check_collision(test)) break;
                nxt = test;
            }
            cur = nxt;
            return true;
        }
        if (operation == "a" || operation == "d" || operation == "s") {
            Tetromino nxt = cur;
            if (operation == "a") nxt.x -= 1;
            else if (operation == "d") nxt.x += 1;
            else nxt.y += 1; // 's'
            if (!check_collision(nxt)) { cur = nxt; return true; }
            return false;
        }
        if (operation == "w") {
            Tetromino nxt = cur;
            const auto &shapeNow = Tetromino::rotateShapes[cur.index][cur.rot];
            nxt.rot = (cur.rot + 1) & 3;
            nxt.x += shapeNow.rotateX;
            nxt.y += shapeNow.rotateY;
            if (!check_collision(nxt)) { cur = nxt; return true; }
            return false;
        }
        return false;
    }

    void Tetris::place_tetris() {
        const auto &mat = Tetromino::rotateShapes[cur.index][cur.rot].shape;
        int h = (int)mat.size();
        int w = (int)mat[0].size();
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if (mat[i][j]) {
                    int yy = cur.y + i;
                    int xx = cur.x + j;
                    if (0 <= yy && yy < HEIGHT && 0 <= xx && xx < WIDTH) field[yy][xx] = 1;
                }
            }
        }
        has_active = false;
    }

    void Tetris::clear_lines() {
        int cleared = 0;
        for (int r = HEIGHT - 1; r >= 0; --r) {
            bool full = true;
            for (int c = 0; c < WIDTH; ++c) if (!field[r][c]) { full = false; break; }
            if (full) {
                ++cleared;
                for (int rr = r; rr > 0; --rr) field[rr] = field[rr - 1];
                field[0].assign(WIDTH, 0);
                ++r; // recheck same row after shift
            }
        }
        if (cleared == 1) score += 100;
        else if (cleared == 2) score += 300;
        else if (cleared == 3) score += 600;
        else if (cleared >= 4) score += 1000;
    }

    void Tetris::overlay_to(std::vector<std::vector<int>>& out) const {
        if (!has_active) return;
        const auto &mat = Tetromino::rotateShapes[cur.index][cur.rot].shape;
        int h = (int)mat.size();
        int w = (int)mat[0].size();
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if (mat[i][j]) {
                    int yy = cur.y + i;
                    int xx = cur.x + j;
                    if (0 <= yy && yy < HEIGHT && 0 <= xx && xx < WIDTH) out[yy][xx] = 1;
                }
            }
        }
    }

    void Tetris::print_field() {
        std::vector<std::vector<int>> tmp = field;
        overlay_to(tmp);
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if (j) std::cout << ' ';
                std::cout << tmp[i][j];
            }
            std::cout << '\n';
        }
    }

    bool Tetris::game_over() {
        for (int j = 0; j < WIDTH; ++j) if (field[0][j]) return true;
        return false;
    }

    void Tetris::run() {
        if (!new_tetris()) {
            std::cout << "Game Over!\n";
            std::cout << "Your score: " << score << "\n";
            return;
        }

        std::string cmd;
        std::string line;
        std::getline(std::cin, line); // consume after seed
        while (true) {
            print_field();
            // read a valid operation
            while (true) {
                if (!std::getline(std::cin, cmd)) cmd = "e";
                // trim
                while (!cmd.empty() && std::isspace((unsigned char)cmd.back())) cmd.pop_back();
                size_t l=0; while (l<cmd.size() && std::isspace((unsigned char)cmd[l])) ++l;
                cmd = cmd.substr(l);
                if (cmd=="w"||cmd=="a"||cmd=="d"||cmd=="s"||cmd=="e"||cmd=="esc"||cmd=="space") break;
            }
            if (cmd == "esc") {
                std::cout << "See you!\n";
                std::cout << "Your score: " << score << "\n";
                return;
            }
            bool ok = move_tetris(cmd);
            std::cout << (ok ? "Successful operation!\n" : "Failed operation!\n");

            if (!dropping()) {
                place_tetris();
                clear_lines();
                if (game_over()) {
                    print_field();
                    std::cout << "Game Over!\n";
                    std::cout << "Your score: " << score << "\n";
                    return;
                }
                if (!new_tetris()) {
                    std::cout << "Game Over!\n";
                    std::cout << "Your score: " << score << "\n";
                    return;
                }
            }
        }
    }

}

#endif //T3_ACMOJ2455_TETRIS_HPP
