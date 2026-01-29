#include <iostream>
using namespace std;

int main()
{
    for (int A = 0; A <= 1; A++)
        for (int B = 0; B <= 1; B++)
            for (int C = 0; C <= 1; C++)
                for (int D = 0; D <= 1; D++)
                    for (int E = 0; E <= 1; E++) {
                        if ((A || B) && (!A || C) && (!D || E) && (D || !C) && (!E == true))
                            cout << "A=" << A << "  B=" << B << endl;
                    }
    return 0;
}
//A:营业员A偷了手表
//B : 营业员B偷了手表
//C : 作案不在营业时间
//D : B提供的证据正确
//E : 货柜未上锁