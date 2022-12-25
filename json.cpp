#include<iostream>
#include<fstream>
#include"bvh_parser.h"
#include<stack>
#include<vector>

using std::ofstream;
using namespace std;

void coutlevel(int level) {
    for (int i = 0; i < level; i++)
    {
        cout << "\t";
    }
}

void jsonify(joint root, META meta_data) {
    ofstream outfile;
    outfile.open("output.json");
    
    // output the root and meta_data
    cout.rdbuf(outfile.rdbuf());//redirect cout to outfile
    stack <joint*> sign_S;
    joint* cur_joint;
    joint start1, end1, end2, comma;
    int level = 0;

    cout << "{" << endl;
    level++;
    
    coutlevel(level);
    cout << "\"frame\": " << meta_data.frame << "," << endl;
    coutlevel(level);
    cout << "\"frame_time\": " << meta_data.frame_time << "," << endl;
    coutlevel(level);
    cout << "\"joint\":" << endl;
    level++;
//joint:
    sign_S.push(&end1);
    sign_S.push(&root);
    sign_S.push(&start1);

    while (!sign_S.empty())
    {
        cur_joint = sign_S.top();
        sign_S.pop();

        if (cur_joint == &start1)
        {
            coutlevel(level);
            cout << "{" << endl;
            level++;
            continue;
        }
        if (cur_joint == &end1)
        {
            level--;
            coutlevel(level);
            cout << "}" << endl;
            continue;
        }
        if (cur_joint == &end2)
        {
            cout << endl;
            level--;
            coutlevel(level);
            cout << "]" << endl;
            continue;
        }
        if (cur_joint == &comma)
        {
            cout << "," << endl;
            continue;
        }

        //"type"
        coutlevel(level);
        cout << "\"type\": \"" << cur_joint->joint_type << "\"," << endl;

        //"name"
        coutlevel(level);
        cout << "\"name\": \"" << cur_joint->name << "\"," << endl;

        //"offset"
        coutlevel(level);
        cout << "\"offset\": [" << cur_joint->offset_x << ", " << cur_joint->offset_y << ", " << cur_joint->offset_z << "]," << endl;

        //"channels"
        coutlevel(level);
        cout << "\"channels\": [";
        int chls = cur_joint->channels.size();
        for (int i = 0; i < chls; i++)
        {
            if (i != 0)
                cout << ", ";
            cout << "\"" << cur_joint->channels[i] << "\"";
        }
        cout << "]," << endl;

        //"motion"
        coutlevel(level);
        cout << "\"motion\": " << "[" << endl;
        level++;
        int size = cur_joint->motion.size();
        for (int i = 0; i < size; i++)
        {
            coutlevel(level);
            cout << "[";
            for (int j = 0; j < chls; j++)
            {
                if (j != 0)
                    cout << ", ";
                cout << cur_joint->motion[i][j];
            }
            cout << "]";
            if (i != size - 1)
                cout << ",";
            cout << endl;
        }
        level--;
        coutlevel(level);
        cout << "]," << endl;

        //"children"
        coutlevel(level);
        cout << "\"children\": [" << endl;
        level++;
        sign_S.push(&end2);
        if (!cur_joint->children.empty())
        {
            for (vector<joint*>::reverse_iterator iter = cur_joint->children.rbegin(); iter != cur_joint->children.rend(); ++iter)
            {
                sign_S.push(&end1);
                sign_S.push(*iter);
                sign_S.push(&start1);

                if (iter != cur_joint->children.rend() - 1)
                {
                    sign_S.push(&comma);
                }
            }
        }
    }

    cout << "}" << endl;
    outfile.close();
}