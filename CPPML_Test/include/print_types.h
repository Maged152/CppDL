#pragma once
#include "shakhbat_ml.h"

inline std::ostream& operator << (std::ostream& out, const qlm::Norm& norm)
{
    switch (norm)
    {
    case qlm::Norm::L1_NORM :
        out << "L1_NORM";
        break;

    case qlm::Norm::L2_NORM:
        out << "L2_NORM";
        break;

    case qlm::Norm::INF_NORM:
        out << "INF_NORM";
        break;
    }

    return out;
}