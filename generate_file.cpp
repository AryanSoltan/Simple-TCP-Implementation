#include<bits/stdc++.h>

using namespace std;

int main()
{
    srand(time(0));
    ofstream outfile;
    outfile.open("huge_file.txt");
    for(int i = 1; i <= 9e5 + 4e4; i++)
    {
        int tmp = rand() % 10 + 1;
        outfile << tmp;
    }
}