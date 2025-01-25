module;
/*
    eLyKseeR or LXR - cryptographic data archiving software
    https://github.com/eLyKseeR/elykseer-cpp
    Copyright (C) 2019-2025 Alexander Diemand

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cmath>
#include <stdint.h>
#include <vector>

import lxr_random;


module lxr_randlist;


namespace lxr {

std::vector<int> mklist(int lo0, int hi0)
{
    int step = 1;   // the increment
    int lo = lo0;
    int hi = hi0;
    if (lo0 > hi0) {
        step = -1;
    }
    int n = abs(hi - lo) + 1;
    std::vector<int> res(n);
    int i = 0;
    int d = lo;
    while (i < n) {
        res[i++] = d;
        d += step;
    }
    return res;
}

std::vector<int> permutation(std::vector<int> vs)
{
    int n = vs.size();
    for (int i=0; i<n; i++) {
        uint32_t r = Random::rng().random(n);
        if (i == r || r < 0 || r >= n) { r = n - i; }
        // swap i with r positions
        auto t = vs[i];
        vs[i] = vs[r];
        vs[r] = t;
    }
    return vs;
}

std::vector<int> RandList::Make(int lo, int hi)
{
    return permutation(mklist(lo, hi));
}

} // namespace