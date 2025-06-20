#ifndef DATABASETEST_H
#define DATABASETEST_H

#include <QObject>
#include "usermanager.h"

class DatabaseTest : public QObject {
    Q_OBJECT

public:
    DatabaseTest(UserManager *userManager);
    void runAllTests();

private:
    void testUserAllocation();
    void testDeleteUser();
    void testGameHistoryRetrieval();
    void resetUserManager(); // Added to clear users list
    UserManager *userManager;
};

#endif // DATABASETEST_H
