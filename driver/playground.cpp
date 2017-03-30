//
// Created by tripack on 17-3-29.
//

#include <common.h>
#include <resource.h>
#include "../MFFC/interface.h"

int main() {
    Path p = ProjectBase.sub("circuit/other_fraig");
    BlifBooleanNet net(p / "C880.blif");
    net.profileBySimulation(10);
    return 0;
}