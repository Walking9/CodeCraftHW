//
// Created by brunon on 18-4-10.
//

#ifndef ECS_REFUSE_H
#define ECS_REFUSE_H

/*

int ExponentialSmooth3fix(const vector<int> data, int n, int k) {   //公式不适合这种方法
    int DataNum = n / k, tempN = n;
    //数据预处理............................没有好的方法暂时不对数据预处理了
    int avg = 0;
    for (unsigned int i=0; i<data.size(); i++) {
        avg += data[i];
    }
    avg = (int)round((double)avg/n);

    int *DataHandled = new int[DataNum];
    for (int i = DataNum - 1; i >= 0; i--) {
        DataHandled[i] = 0;
        for (int j = 0; j < k; j++) {
            tempN--;
            if(0 != avg && data[tempN] > 5*avg) DataHandled[i] += avg;
            else DataHandled[i] += data[tempN];
        }
    }
    vector<double> s3_1_new, s3_2_new, s3_3_new;
    double x = 0;
    for (int i = 0; i < 3; i++)
        x += DataHandled[i];
    x /= 3;
    double s3_1 = x, s3_2 = x, s3_3 = x;  //初始化s值
    double a = 0.1, a_fit;      //平滑系数!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    double Xt;
    double Difference = INTMAX_MAX, nowDifference;
    for(int i=0; i<90; i++) {
        double tempDouble;

        //先计算一次指数平滑的值
        for (int i = 0; i < DataNum-1; i++) {
            if (0 == i) {
                s3_1_new.push_back(a * DataHandled[i] + (1 - a) * s3_1);
            } else {
                s3_1_new.push_back(a * DataHandled[i] + (1 - a) * s3_1_new[i - 1]);
            }
        }

        //再计算二次指数平滑的值
        for (int i = 0; i < DataNum-1; i++) {
            if (0 == i) {
                s3_2_new.push_back(a * s3_1_new[i] + (1 - a) * s3_2);
            } else {
                s3_2_new.push_back(a * s3_1_new[i] + (1 - a) * s3_2_new[i - 1]);
            }
        }

        //后计算三次指数平滑值
        for (int i = 0; i < DataNum-1; i++) {
            if (0 == i) {
                s3_3_new.push_back(a * s3_2_new[i] + (1 - a) * s3_3);
            } else {
                s3_3_new.push_back(a * s3_2_new[i] + (1 - a) * s3_3_new[i - 1]);
            }
        }
        s3_1 = s3_1_new[s3_1_new.size() - 1];
        s3_2 = s3_2_new[s3_2_new.size() - 1];
        s3_3 = s3_3_new[s3_3_new.size() - 1];
        double At = s3_1 * 3 - s3_2 * 3 + s3_3;
        double Bt = (a / (2 * (1 - a) * (1 - a))) * ((6 - 5 * a) * s3_1 - (10 - 8 * a) * s3_2 + (4 - 3 * a) * s3_3);
        double Ct = ((a * a) / (2 * (1 - a) * (1 - a))) * (s3_1 - 2 * s3_2 + s3_3);
        Xt = At + Bt + Ct;
        nowDifference = abs(DataHandled[DataNum-1] - Xt);
        if(nowDifference < Difference) {
            a_fit = a;
        }
        s3_1_new.clear();
        s3_2_new.clear();
        s3_3_new.clear();

        a += 0.01;      //平滑系数!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    a = a_fit;

    //先计算一次指数平滑的值
    for (int i = 0; i < DataNum; i++) {
        if (0 == i) {
            s3_1_new.push_back(a * DataHandled[i] + (1 - a) * s3_1);
        } else {
            s3_1_new.push_back(a * DataHandled[i] + (1 - a) * s3_1_new[i - 1]);
        }
    }

    //再计算二次指数平滑的值
    for (int i = 0; i < DataNum; i++) {
        if (0 == i) {
            s3_2_new.push_back(a * s3_1_new[i] + (1 - a) * s3_2);
        } else {
            s3_2_new.push_back(a * s3_1_new[i] + (1 - a) * s3_2_new[i - 1]);
        }
    }

    //后计算三次指数平滑值
    for (int i = 0; i < DataNum; i++) {
        if (0 == i) {
            s3_3_new.push_back(a * s3_2_new[i] + (1 - a) * s3_3);
        } else {
            s3_3_new.push_back(a * s3_2_new[i] + (1 - a) * s3_3_new[i - 1]);
        }
    }
    s3_1 = s3_1_new[s3_1_new.size() - 1];
    s3_2 = s3_2_new[s3_2_new.size() - 1];
    s3_3 = s3_3_new[s3_3_new.size() - 1];
    double At = s3_1 * 3 - s3_2 * 3 + s3_3;
    double Bt = (a / (2 * (1 - a) * (1 - a))) * ((6 - 5 * a) * s3_1 - (10 - 8 * a) * s3_2 + (4 - 3 * a) * s3_3);
    double Ct = ((a * a) / (2 * (1 - a) * (1 - a))) * (s3_1 - 2 * s3_2 + s3_3);
    Xt = At + Bt + Ct;

#ifdef _DEBUG
//    printf("未来%d期的二次指数平滑预估值为： %lf, 均方误差为： %lf\n",forecase, Xt, MSE);
    cout << "\n实际数据:";
    for(int i=0; i<DataNum; i++) {
        cout << DataHandled[i] << " ";
    }
    cout << endl;
    cout << "指数平滑:";
    for(int i=0;i<s3_3_new.size();i++) {
        cout << s3_3_new[i] <<" ";
    }
    cout << "\npredict num = " << Xt << " 平滑系数：" << a  << "\n\n"<< endl;
#endif
    delete[] DataHandled;
    if(Xt > 0) return (Xt - (int)Xt) >= 0.45 ? (int)ceil(Xt) : (int)Xt;     //0.35~0.45 > round > ceil, 最高分76.182(不对数据处理76.173)
    else return 0;
//    if(Xt > 0) return (int)round(Xt);    //ceil:72.434, round:75.809
//    else return 0;
}
 * */


#endif //ECS_REFUSE_H
