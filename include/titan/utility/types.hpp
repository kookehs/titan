// Project Titan
//
// To the extent possible under law, the person who associated CC0 with
// Project Titan has waived all copyright and related or neighboring rights
// to Project Titan.
//
// You should have received a copy of the CC0 legalcode along with this
// work. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#ifndef TITAN_UTILITY_TYPES_HPP_
#define TITAN_UTILITY_TYPES_HPP_

struct vector_2f {
        float x = 0.0f;
        float y = 0.0f;
};

struct rect_f {
        float x = 0.0f;
        float y = 0.0f;
        float width = 0.0f;
        float height = 0.0;
};

struct rect_i {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
};

#endif /* TITAN_UTILITY_TYPES_HPP_ */
