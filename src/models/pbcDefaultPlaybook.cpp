//
// Created by oliver on 7/3/19.
//

#include "models/pbcRoute.h"
#include "models/pbcFormation.h"

static void default_routes(PBCModelMap<PBCRouteSP> &routes) {
    PBCRouteSP route_5In(new PBCRoute("5 In", "", {PBCPathSP(new PBCPath(0, 5)),
                                                   PBCPathSP(new PBCPath(10, 5))}));
    routes.insert(std::make_pair("5 In", route_5In));

    std::vector<PBCPathSP> path_Post({ PBCPathSP(new PBCPath(0, 5)),
                                       PBCPathSP(new PBCPath(2, 7))});
    PBCRouteSP route_Post(new PBCRoute("Post", "", path_Post));
    routes.insert(std::make_pair(route_Post->name(), route_Post));


    std::vector<PBCPathSP> path_Slant({ PBCPathSP(new PBCPath(0, 2)),
                                        PBCPathSP(new PBCPath(2, 4))});
    PBCRouteSP route_Slant(new PBCRoute("Slant", "", path_Slant));
    routes.insert(std::make_pair(route_Slant->name(), route_Slant));


    std::vector<PBCPathSP> path_Fly({ PBCPathSP(new PBCPath(0, 7))});
    PBCRouteSP route_Fly(new PBCRoute("Fly", "", path_Fly));

    routes.insert(std::make_pair(route_Fly->name(), route_Fly));
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