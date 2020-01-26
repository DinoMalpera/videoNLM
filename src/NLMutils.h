#pragma once

/*  Common helpers 
 * */

namespace VNLM
{

    /*  Square of a squareable type.
     * */
    template <typename T>
    static
    constexpr
    T
    sqr(
            const T _ ) noexcept
    {
        return _ * _;
    }

}
