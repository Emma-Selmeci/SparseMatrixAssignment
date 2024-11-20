#include <iostream>
#include "sparsematrix.hpp"
#include "woodpecker.hpp"
#include "SparseMatrixExceptions.hpp"

TEST("Egyszeru ures"){
    SparseMatrix s = SparseMatrix(1000,1000,0,96);
    CHECK_EQ(s(501,104),0);
    CHECK_EQ(s(99,71),0);
}
TEST("Meglevo ertek torlese"){
    SparseMatrix s = SparseMatrix(1000,1000,9,31);
    CHECK_NOEXC(s.set(100,410,33));
    CHECK_NOEXC(s.clear(100,410));
    CHECK_EQ(s(100,410),9);
}

TEST("Meglevo ertek felulirasa"){
    SparseMatrix s = SparseMatrix(100,100,9,256);
    s.set(23,32,45);
    CHECK_EQ(s(23,32),45);
    s.set(23,32,19);
    CHECK_EQ(s(23,32), 19);
}
TEST("Kis matrix ertekekkel"){
    SparseMatrix s = SparseMatrix(10,10,1,5);
    s.set(5,5,2.2);
    s.set(5,4,2.1);
    s.set(2,8,2.8);
    s.printMatrix();
    CHECK_EQ(s(5,5), 2.2);
    CHECK_EQ(s(5,4), 2.1);
    CHECK_EQ(s(2,8), 2.8);
    CHECK_EQ(s(0,0), 1);
    CHECK_EQ(s(0,1), 1);
    CHECK_EQ(s(0,2), 1);

    s.set(5,5,1);
    s.clear(5,4);
    CHECK_EQ(s(5,5), s(5,4)); // helyes
}
TEST("Nagy matrix ertekekkel"){
    SparseMatrix s = SparseMatrix(1000,1000,0,254);
    s.set(884, 12, 777);
    s.set(0, 442, 998);
    s.set(200, 20, 10.1);
    CHECK_EQ(s(111,10),0);
    CHECK_EQ(s(999,42),0);
    CHECK_EQ(s(884, 12),777);
    CHECK_EQ(s(0, 442),998);
    CHECK_EQ(s(200, 20),10.1);
}
TEST("Egyszeru ertekekkel - nemnegyzetes"){
    SparseMatrix s = SparseMatrix(1000,100,0,53);
    s.set(865, 12, 93);
    s.set(0, 29, 998);
    CHECK_EQ(s(100,10),0);
    CHECK_EQ(s(999,42),0);
    CHECK_EQ(s(865, 12),93);
    CHECK_EQ(s(0, 29),998);
}
TEST("Egyszeru ertekekkel es eltavolitassal"){
    SparseMatrix s = SparseMatrix(1000,1000,0,252);
    for(int i = 0; i < 100; i++){
        s.set(200+i, 40+2*i, i+5);
    }
    s.set(88, 2, 42);
    s.set(0, 449, 798);
    CHECK_EQ(s(100,10),0);
    CHECK_EQ(s(999,42),0);
    CHECK_EQ(s(88, 2),42);
    CHECK_EQ(s(0, 449),798);
    for(int i = 0; i < 100; i++){
        CHECK_EQ(s(200+i, 40+2*i),i+5);
        CHECK_EQ(s(199, 40+2*i),0);
    }
    for(int i = 0; i < 100; i++){
        s.clear(200+i, 40+2*i);
    }
    for(int i = 0; i < 100; i++){
        CHECK_EQ(s(200+i, 40+2*i),0);
        CHECK_EQ(s(199, 40+2*i),0);
    }
}

TEST("Egyszeru ertekekkel - kiindexeles"){
    SparseMatrix s = SparseMatrix(123,321,3,68);
    CHECK_EXC(IndexOutOfRangeException, s(1000,1));
    CHECK_EXC(IndexOutOfRangeException, s.set(300,400, 12));
    CHECK_EXC(IndexOutOfRangeException, s.clear(0,-1));
}
TEST("Matrix nemnulla default ertekkel"){
    SparseMatrix s = SparseMatrix(1000,1000,42,250);
    s.set(885, 12, 40);
    s.set(0, 442, 998);
    CHECK_EQ(s(100,10),42);
    CHECK_EQ(s(999,42),42);
    CHECK_EQ(s(885, 12),40);
    CHECK_EQ(s(0, 442),998);
    for(int i = 0; i < 100; i++){
        CHECK_EQ(s(200+i, 40+2*i),42);
    }
}
TEST("Matrix osszeadas - ures matrixok") {
    SparseMatrix s1 = SparseMatrix(1000,1000,0,42);
    SparseMatrix s2 = SparseMatrix(1000,1000,0,49);
    SparseMatrix s3 = s1 + s2;
    CHECK_EQ(s3(170,109),0);
    CHECK_EQ(s3(99,739),0);
    CHECK_EQ(s3(376,749),0);
}
TEST("Kis matrix osszeadas") {
    SparseMatrix s1 = SparseMatrix(10,10,0,32);
    SparseMatrix s2 = SparseMatrix(10,10,0,64);
    s1.set(2,2,2.2);
    s1.set(8,1,53.1);
    s1.set(5,7,9.8);
    s2.set(2,4,98.3);
    s2.set(8,1,1.1);
    SparseMatrix s3 = s1 + s2;
    CHECK_EQ(s3(2,2),2.2);
    CHECK_EQ(s3(8,1),54.2);
    CHECK_EQ(s3(5,7),9.8);
    CHECK_EQ(s3(2,4),98.3);
    CHECK_EQ(s3(8,2),0);
    CHECK_EQ(s3(0,0),0);
}

TEST("Nagy matrix osszeadas - nemnegyzetes") {
    SparseMatrix s1 = SparseMatrix(1000,400,0,78);
    SparseMatrix s2 = SparseMatrix(1000,400,0,13);
    s1.set(2,2,2.3);
    s1.set(8,1,63.1);
    s1.set(5,7,9.8);
    s1.set(907,399,444.4);
    s2.set(2,4,98.3);
    s2.set(8,1,1.1);
    s2.set(907,399,37.2);
    SparseMatrix s3 = s1 + s2;
    CHECK_EQ(s3(2,2),2.3);
    CHECK_EQ(s3(8,1),64.2);
    CHECK_EQ(s3(5,7),9.8);
    CHECK_EQ(s3(2,4),98.3);
    CHECK_CLOSE_TO(s3(907,399),481.6, 0.01);
    CHECK_EQ(s3(8,2),0);
    CHECK_EQ(s3(0,0),0);
    CHECK_EQ(s3(999,399),0);
}

TEST("Matrix osszeadas - nemnulla default ertekkel") {
    SparseMatrix s1 = SparseMatrix(1000,1001,79,25);
    SparseMatrix s2 = SparseMatrix(1000,1001,15,27);
    s1.set(884, 13, 31);
    s2.set(884, 13, 49);
    s1.set(0, 441, 997);
    s2.set(0, 441, 999);
    s1.set(583, 876, 679.3);
    s2.set(943, 1000, 92.5);
    CHECK_NOEXC(s1 + s2);
    SparseMatrix s3 = s1 + s2;
    CHECK_EQ(s3.getDefaultValue(), 94);
    CHECK_EQ(s3(100,10),94);
    CHECK_EQ(s3(999,41),94);
    CHECK_EQ(s3(884, 13),80);
    CHECK_EQ(s3(0, 441),1996);
    CHECK_EQ(s3(583, 876),694.3);
    CHECK_EQ(s3(943, 1000),171.5);
}
TEST("Matrix osszeadas - meretek nem egyeznek") {
    SparseMatrix s1 = SparseMatrix(10,70,0,9);
    SparseMatrix s2 = SparseMatrix(1,101,6,6);
    CHECK_EXC(SizeMismatchException, s1 + s2);
}
TEST("Ido check - ures matrixok") {
    SparseMatrix s1 = SparseMatrix(1000,1000,1,15);
    SparseMatrix s2 = SparseMatrix(1000,1000,2,25);
    TIMER(ido) {
        s1 + s2;
    }
    std::cout << "SparseMatrix osszeadas: " << ido.count() << " ns" << std::endl;
    TIMER(ido2) {
        s1.add(s2);
    }
    std::cout << "Matrix osszeadas: " << ido2.count() << " ns" << std::endl;
    CHECK_EQ(ido.count() * 1000 < ido2.count(), true);
}
TEST("Ido check") {
    SparseMatrix s1 = SparseMatrix(1000,1000,5,12);
    SparseMatrix s2 = SparseMatrix(1000,1000,3,22);
    for (int i = 0; i < 50; ++i) {
        for (int j = 0; j < 50; ++j) {
            s1.set(i*2, j*2, i*2.5 + j*3.3);
            s2.set(i*4, j*4, i*1.7 + j*9.2);
        }
    }
    TIMER(ido) {
        s1 + s2;
    }
    std::cout << "SparseMatrix osszeadas: " << ido.count() << " ns" << std::endl;
    TIMER(ido2) {
        s1.add(s2);
    }
    std::cout << "Matrix osszeadas: " << ido2.count() << " ns" << std::endl;
    CHECK_EQ(ido.count() * 10 < ido2.count(), true);
}
TEST("Ido check - merettol fuggetlen") {
    SparseMatrix large1 = SparseMatrix(10000,10000,11,3);
    SparseMatrix large2 = SparseMatrix(10000,10000,13,5);
    SparseMatrix small1 = SparseMatrix(10,10,11,3);
    SparseMatrix small2 = SparseMatrix(10,10,13,5);

    large1.set(1109, 4329, 983.1);
    large1.set(837, 7813, 46.6);
    large1.set(5001, 5001, 51.5);
    large2.set(2475, 6879, 235.4);
    large2.set(3977, 8042, 2024.2);
    large2.set(5001, 5001, 48.5);

    small1.set(3, 2, 204.3);
    small1.set(4, 1, 58.9);
    small1.set(6, 6, 5.9);
    small2.set(9, 5, 714.3);
    small2.set(8, 7, 99.1);
    small2.set(6, 6, 4.1);

    TIMER(t){large1 + large2;small1 + small2;} // Elso futtatas utan a tobbi mar gyorsabb

    TIMER(tLarge) {
        large1 + large2;
    }
    std::cout << "Nagy SparseMatrix osszeadas: " << tLarge.count() << " ns" << std::endl;

    TIMER(tSmall) {
        small1 + small2;
    }
    std::cout << "Kis SparseMatrix osszeadas: " << tSmall.count() << " ns" << std::endl;

    CHECK_EQ(std::abs(tLarge.count() - tSmall.count()) < 500, true);
}


WOODPECKER_TEST_MAIN()