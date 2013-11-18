// log error msg, stop execution
void ErrorFatal(const string& errmsg)
{
    eraseDisplay();
    nxtDisplayCenteredTextLine(3, "ERROR");
    nxtDisplayCenteredTextLine(4, errmsg);
    wait1Msec(3000);
    StopAllTasks();
}


// log msg
void LogMsg(const string& msg)
{
    eraseDisplay();
    nxtDisplayCenteredTextLine(3, "LOG");
    nxtDisplayCenteredTextLine(4, msg);
    wait1Msec(1000);
}


