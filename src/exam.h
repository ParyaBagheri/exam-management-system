#include<bits/stdc++.h>
#include<vector>
#include<cctype>
#include <tuple>
#include<string>
#include<cmath>
#include"json.hpp"
#include<algorithm>
#include<fstream>

using json = nlohmann::json;
using namespace std;

extern string command;

struct StudentGrades{
    string name;
    string id;
    float grade;
};

class StudentsGrade{
    public:
    string student_id;
    float grade;
};
enum AnswerStatus{correct = 1, not_answered, incorrect};
enum QuestionType{Multiple_choice = 1 ,Essay_question, Short_answer};
class Question{
    protected:
    string text;
    string answer;
    string givenAnswer;
    string choices[4];
    float points_positive;
    float points_negative;
    AnswerStatus answer_status;
    QuestionType type;
    public:
    Question(){
        answer_status = not_answered;
    };
    inline void set_choices(){
        string choice;
        cout<<"Enter choices : "<<endl;
        for(int i = 0; i < 4; i++){
            getline(cin, choice);
            choices[i] = choice;
        }
        return;
    }
    json Questions_toJson() const;
    void view_choices();
    inline void set_type(int type){this->type = static_cast<QuestionType>(type);};
    inline void set_text(string t){ this->text = t;}
    inline void set_answer(string answer){ this->answer = answer; }
    inline void check_answer(string given_answer){
        if(given_answer == answer)
            answer_status = correct;
        else{
            answer_status = incorrect;
        }
    }
    inline void set_points(float positive_points, float negative_points){ points_positive = positive_points;
        points_negative = negative_points; }
    inline void set_points(float positive_points){ points_positive = positive_points;
        points_negative = 0; }
    inline void show_t(){
        int idx = 1;
        cout<<idx++<<". "<<text<<"   "<<"Answer : "<<answer<<endl;
    }
    inline string show_s(){
        return text;
    }
    // inline int is_correct(){
    //     return iscorrect;
    // }
    friend class Exam;
};
class Exam {
    protected:
    static vector<string> all_codes;
    static int Exam_count;
    string name;
    string code;
    string creator_id;
    vector<Question> questionlist;
    vector<tuple<string, string, int>> student_grades;
    float whole_grade;
    float grade;
    public:
    Exam(){
        grade = 0;
        //code = " ";
    };
    inline void set_name(string name){this->name = name;};
    inline void set_creatorID(string id){creator_id = id;};
    inline string get_creatorID(){return creator_id;};
    inline string get_ExamCode(){return code;};
    void Add_question();
    void Remove_question();
    json ExamtoJson();
    void Addexamdatatofile();
    string code_generator();//generate a unique code and store it in the all codes vector!!!
    //void Add_question(string text, float answer, float points);
    //void Remove_question(int index);
    void SaveExam(json* j);
    void view_questions_t();
    void view_questions_s();
    void Answer_question(string answer);
    float calculate_grade();
    friend class teacher;
    friend class student;
};

class User{
    protected:
    //static int user_count;
    string FirstName;
    string LastName;
    string id;
    string password;
    string occupation;
    vector<Exam> exams;
    vector<string> exam_codes;
    int exam_count;
    public:
    User(){
        exam_count = 0;
        //user_count++;
    }
    ~User() = default;
    virtual void signup() {}
    //virtual bool login();
    //virtual bool login(json& userdata);
    virtual json toJson() { return{}; }
    virtual void view_Exams() {};
    virtual void Adduserdatatofile();
    inline string get_name(){return FirstName+LastName; };
    inline string get_id(){return id;};
    inline string get_password(){return password;};
    inline void set_name(string Firstname, string Lastname){ FirstName = Firstname; LastName = Lastname;};
    inline void set_id(string id){this->id = id;};
    inline void set_occupation(string occupation){this->occupation = occupation;}
    inline void set_password(string password){this->password = password;};
    inline void set_examcodes(vector<string>& exam_codes){ this->exam_codes = exam_codes; }
    virtual void show_exam(json& exam_data, string exam_code) {};
    //void panel_redirect(json &j);
    virtual void teacher_panel() {};
    virtual void student_panel() {};
}; 
 
//vector<Exam> all;

class teacher : public User{
    protected :
    //static int teachers_count;
    vector<string> courses;
    vector<string> exam_codes;
    public:
    teacher(){
        exam_count = 0;
        //teachers_count++;
        occupation = "teacher";
    }
    ~teacher() = default;
    void signup() override;
    void teacher_panel() override;
    void create_exam();
    void set_courses(string c);
    void view_Exams();
    //void create_Exam(string ename);
    //void add_question(string text, float answer, float point);
    //void remove_question(int index);
    //void show_details(int index);
    //void TeacherPanel(string id);
    void show_exam(json& exam_data, string exam_code);    
    json toJson();
    void showGradeList(string exam_code);
};

class student : public User{
    protected:
    //static int students_count;
    string major;
    public:
    student(){
        //students_count++;
        exam_count = 0;
        occupation = "student";
    }
    ~student() = default;
    inline void set_major(string major){this->major = major;};
    void signup() override;
    void student_panel() override;
    //void add_Exam(string code);
    void add_exam();
    void view_Exams();
    void take_Exam();
    //void show_exam(json &exam_data, string exam_code);
    //void take_Exam(string c);
    //void StudentPanel(string id);
    json toJson();
    float exam_checker(json& exam_data, string exam_code);
    void showStudentResault(string exam_code);
};
class UserPanels{
    protected:
    string user_id;
    public:
    virtual void showPanel(string id);
};
class StudentPanel : public UserPanels{
    public : 
    void showPanel(string id) override;
    void create_exam();

};
class TeacherPanel : public UserPanels{
    void showPanel(string id) override;
};
class Resault{
    protected:
    string exam_code;
    string student_id;
    string students_name;
    float grade;
    float gradeOfMCQs;
    float gradeOfShortAnswers;
    vector<string> checked_answers;
    public:
    Resault();
    Resault(string exam_code ,string student_id,string students_name ,float grade, float gradeOfMCQs, float gradeOfShortAnswers,vector<string>& checked_answers){
        this->exam_code = exam_code;
        this->student_id = student_id;
        this->students_name = students_name;
        this->grade = grade;
        this->gradeOfMCQs = gradeOfMCQs;
        this->gradeOfShortAnswers = gradeOfShortAnswers;
        this->checked_answers = checked_answers;
    }
    json toJson();
    //void saveExamResaults();
    void saveStudentResaults();

};
//int User::user_count = 0;
//int teacher::teachers_count = 0;
//int student::students_count = 0;
//int Exam::Exam_count = 0;

