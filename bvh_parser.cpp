#include<fstream>
#include<iostream>
#include"bvh_parser.h"
#include<stack>
#include<vector>
#include<string>
// a naive bvh parser

using std::ifstream;
using namespace std;

int main(int argc, char** argv) {
    joint root;
    META meta_data;
    ifstream file(argv[1]);

    //do something
    file.open(argv[1]);
    cin.rdbuf(file.rdbuf());
    
    stack <joint*> sign_S;
    joint* cur_joint;
    string temp;
    sign_S.push(&root);

    while (!sign_S.empty())
    {
        cur_joint = sign_S.top();
        cin >> temp;

        if (temp == "ROOT")
        {
            cin >> cur_joint->name;
            cur_joint->joint_type = "ROOT";
        }
        else if (temp == "JOINT" || temp == "End")
        {
            joint* newchild = new joint;
            cur_joint->children.push_back(newchild);
            sign_S.push(newchild);
            if (temp == "End")
            {
                newchild->name = cur_joint->name + "_End";
                newchild->joint_type = "End";
            }
            else if (temp == "JOINT")
            {
                cin >> newchild->name;
                newchild->joint_type = "JOINT";
            }
        }
        else if (temp == "OFFSET")
        {
            cin >> cur_joint->offset_x >> cur_joint->offset_y >> cur_joint->offset_z;
        }
        else if (temp == "CHANNELS")
        {
            int counter;
            cin >> counter;
            for (int i = 0; i < counter; i++)
            {
                cin >> temp;
                cur_joint->channels.push_back(temp);
            }
        }
        else if (temp == "}")
        {
            sign_S.pop();
        }
    }

    cin >> temp;
    if (temp == "MOTION")
    {
        char ch;
        do {
            cin >> ch;
        } while (ch != ':');
        cin >> meta_data.frame;
        do {
            cin >> ch;
        } while (ch != ':');
        cin >> meta_data.frame_time;
    }

    //pre-order
    for (int frames = 0; frames < meta_data.frame; frames++)
    {
        sign_S.push(&root);

        while (!sign_S.empty())
        {
            cur_joint = sign_S.top();
            sign_S.pop();

            if (!cur_joint->children.empty())
            {
                for (vector<joint*>::reverse_iterator iter = cur_joint->children.rbegin(); iter != cur_joint->children.rend(); ++iter)
                {
                    sign_S.push(*iter);
                }
            }

            vector<double> motion_data;
            if (cur_joint->joint_type == "End")
            {
                cur_joint->motion.push_back(motion_data);
            }
            else
            {
                for (int counter = cur_joint->channels.size(); counter > 0; counter--)
                {
                    double motion_num;
                    cin >> motion_num;
                    motion_data.push_back(motion_num);
                }
                cur_joint->motion.push_back(motion_data);
            }
        }
    }
    
    jsonify(root, meta_data);
    file.close();
    return 0;
}