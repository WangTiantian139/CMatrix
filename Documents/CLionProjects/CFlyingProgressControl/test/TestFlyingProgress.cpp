//
// Created by wang on 18-4-10.
//

#include <iostream>
#include "TestFlyingProgress.h"

using namespace CMotion;

void testFlyingProgress()
{
    std::string file_name = "../test/doc/TestMotionCmd.txt";
    std::ifstream fin(file_name);
    std::ofstream fout;
    if (not fin)
    {
        std::cerr << "open " << file_name << " failed" << std::endl;
    }

    FlyingProgress progress;
    status ori_status;
    Motion motion(ori_status);
    progress.inputCmd(fin);
    fin.close();

    // Create a serial of motion by command
    progress.createMotionByCmd(motion);
    file_name = "../test/doc/TestMotionOutByCmd.csv";
    fout.open(file_name);
    if (not fout)
    {
        std::cerr << "open " << file_name << " failed" << std::endl;
    }
    fout.clear();
    progress.showMotionInFile(motion, fout);
    fout.close();

    // Add a piece of motion and update it
    progress.addStaticAMotion(1, 1, {0, -1, 0});
    progress.addStaticVMotion(15, 3);
    progress.addStaticAMotion(5, {0, -5, 0});
    progress.addStaticAMotion(5, {0, 5, 0});
    progress.addStaticAMotion(1, {-1, 0, 0});
    progress.createMotionByAcc(motion, 1);
    file_name = "../test/doc/TestMotionByOutAcc.csv";
    fout.open(file_name);
    if (not fout)
    {
        std::cerr << "open " << file_name << " failed" << std::endl;
    }
    fout.clear();
    progress.showMotionInFile(motion, fout);
    fout.close();

}
