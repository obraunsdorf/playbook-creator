//
// Created by oliver on 7/3/19.
//

#include "models/pbcRoute.h"
#include "models/pbcFormation.h"

//#define ROUTE(name, pathVector) routes.insert(std::make_pair(name, PBCRouteSP(new PBCRoute(name, "", pathVector))));

static void route(PBCModelMap<PBCRouteSP> &routes, const std::string &name, std::vector<PBCPathSP> paths) {
    routes.insert(std::make_pair(name, PBCRouteSP(new PBCRoute(name, "", paths))));
}

static void default_routes(PBCModelMap<PBCRouteSP> &routes) {
    route(routes, "Hook", {PBCPathSP(new PBCPath(0, 6)),
                           PBCPathSP(new PBCPath(1, 5))});

    route(routes, "Comeback", {PBCPathSP(new PBCPath(0, 12)),
                               PBCPathSP(new PBCPath(-2, 10))});

    route(routes, "5 In", {PBCPathSP(new PBCPath(0, 5)),
                           PBCPathSP(new PBCPath(10, 5))});

    route(routes, "10 In", {PBCPathSP(new PBCPath(0, 10)),
                           PBCPathSP(new PBCPath(10, 10))});

    route(routes, "5 Out", {PBCPathSP(new PBCPath(0, 5)),
                           PBCPathSP(new PBCPath(-10, 5))});

    route(routes, "10 Out", {PBCPathSP(new PBCPath(0, 10)),
                            PBCPathSP(new PBCPath(-10, 10))});

    route(routes, "Slant", {PBCPathSP(new PBCPath(0, 2)),
                            PBCPathSP(new PBCPath(9, 5))});

    route(routes, "Shallow", {PBCPathSP(new PBCPath(13, 2, 2, 2)),
                            PBCPathSP(new PBCPath(15,2))});

    route(routes, "Curl", {PBCPathSP(new PBCPath(0, 12)),
                               PBCPathSP(new PBCPath(2, 10))});

    route(routes, "Post", {PBCPathSP(new PBCPath(0, 7)),
                           PBCPathSP(new PBCPath(7, 14))});

    route(routes, "Corner", {PBCPathSP(new PBCPath(0, 7)),
                           PBCPathSP(new PBCPath(-7, 14))});

    route(routes, "Fly", {PBCPathSP(new PBCPath(-1, 12, -0.7, 3)),
                          PBCPathSP(new PBCPath(-1, 14))});

    route(routes, "Seam", {PBCPathSP(new PBCPath(1, 12, 0.7, 3)),
                           PBCPathSP(new PBCPath(1, 14))});

    route(routes, "Fade", {PBCPathSP(new PBCPath(-1, 5, -0.7, 1)),
                          PBCPathSP(new PBCPath(-1, 7))});
}

static void default_formations(PBCModelMap<PBCFormationSP> &formations, const unsigned int playerNumber) {
    PBCFormationSP spread_right_formation(new PBCFormation("Spread Right"));
    spread_right_formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Center", "C"},
                                                                PBCColor(),
                                                                PBCDPoint(0, 0))));

    spread_right_formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Quarterback", "QB"},
                                                                PBCColor(),
                                                                PBCDPoint(0, -1.5))));

    spread_right_formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Wide Receiver Left", "WRL"},  // NOLINT
                                                                PBCColor(),
                                                                PBCDPoint(-10, 0))));

    spread_right_formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Wide Receiver Right", "WRR"}, // NOLINT
                                                                PBCColor(),
                                                                PBCDPoint(10, 0))));

    spread_right_formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Halfback", "HB"},
                                                                PBCColor(),
                                                                PBCDPoint(5, 0))));

    if (playerNumber >= 7) {
        spread_right_formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Left Guard", "LG"}, // NOLINT
                                                                    PBCColor(),
                                                                    PBCDPoint(-1, 0))));

        spread_right_formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Right Guard", "RG"}, // NOLINT
                                                                    PBCColor(),
                                                                    PBCDPoint(1, 0))));
    }

    if (playerNumber >= 9) {
        spread_right_formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Fullback", "FB"}, // NOLINT
                                                                    PBCColor(),
                                                                    PBCDPoint(0, -3))));

        spread_right_formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Tight End", "TE"}, // NOLINT
                                                                    PBCColor(),
                                                                    PBCDPoint(3, -1))));
    }

    if (playerNumber == 11) {
        spread_right_formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Left Tackle", "LT"}, // NOLINT
                                                                    PBCColor(),
                                                                    PBCDPoint(-2, 0))));

        spread_right_formation->push_back(PBCPlayerSP(new PBCPlayer(PBCRole{"Right Tackle", "RT"}, // NOLINT
                                                                    PBCColor(),
                                                                    PBCDPoint(2, 0))));
    }


    formations.insert(std::make_pair(spread_right_formation->name(), spread_right_formation));
}