#include"Exam.h"

string command = "0";

void teacher::signup(){
    Adduserdatatofile();
    cout<<"Signed up successfully!"<<endl;    
}
void student::signup(){
    Adduserdatatofile();
    cout<<"Signed up successfully!"<<endl;
}
void User::Adduserdatatofile(){
    ifstream inFile("users.json");
    json j;
    if(inFile.good()) inFile >> j;
    inFile.close();
    if(!j.contains("users")){
        j["users"] = json::array();
    }
    j["users"].push_back(toJson());
    ofstream outFile("users.json");
    outFile << j.dump(4);
}
void Exam :: Addexamdatatofile(){
    ifstream inFile("exams.json");
    json j;
    if(inFile.good()) inFile >> j;
    inFile.close();
    if(!j.contains("exams"))
        j["exams"] = json::array();
    j["exams"].push_back(ExamtoJson());
    ofstream ofs_exams("exams.json");
    if(!ofs_exams.good()){
        cout<<"ERROR!"<<endl;
        return;
    }
    ofs_exams << j.dump(4);
}
void teacher::teacher_panel(){
    //string command;
    cout<<"Press 1 to create a new exam."<<endl<<"press 2 to view your exams"<<endl<<"Press x to return to the main page"<<endl;
        do{
            getline(cin, command);
            if(command == "1"){
                create_exam();
            }
            if(command == "2"){
                view_Exams();
            }
        }while(command != "x");
        return;
}
void teacher::create_exam(){
    //json created_exam;
    //ofs_exams << created_exam;
    Exam new_exam;
    string name;
    string code = new_exam.code_generator();
    cout<<"Enter a name for this exam"<<endl;
    getline(cin, name);
    new_exam.set_name(name);
    cout<<"Exam Code : "<<code<<endl;
    new_exam.set_creatorID(id);
    exam_codes.push_back(code);
    string menu;
    do{
        cout<<"1.add question"<<endl<<"2.remove question"<<endl<<"3.END"<<endl;
        getline(cin, menu);
        if(menu == "1"){
            new_exam.Add_question();
        }
        if(menu == "2"){
            new_exam.Remove_question();
        }
        if(menu == "3"){
            new_exam.Addexamdatatofile();
            //created_exam["exams"].push_back(new_exam.ExamtoJson());
            cout<<"Exam was created successfully!"<<endl;
            //new_exam.SaveExam(&created_exam);
            // j["exams"].push_back({"name",name},{"code",code},{"creatorID",creator_id},{"questions",questionlist})
            //ofstream outFile("exams.json");
            //outFile << created_exam.dump(4);
            return;
        }
    }while(menu != "x");
    return;
}
void Exam::Add_question(){
    cout<<"Choose the question type:"<<endl<<"1.Multiple choice"<<endl<<"2.Essay question"<<endl<<"3. Short answer question"<<endl<<"Press x to return"<<endl;
    string text, answer, qtype;
    float positivePoints, negativePoints;
    Question new_question;
    do{
        getline(cin, qtype);
        new_question.set_type(stoi(qtype));
        cout<<"Enter the text of your question : "<<endl;
        getline(cin, text);
        new_question.set_text(text);
        if(stoi(qtype) == 1) new_question.set_choices();
        cout<<"Enter the correct answer for this question"<<endl;
        getline(cin, answer);
        new_question.set_answer(answer);
        if(qtype == "2"){
            cout<<"Enter the number of points for this question"<<endl;
            cin>>positivePoints;
            new_question.set_points(positivePoints);
        }
        else if(qtype == "1" || qtype == "3"){
            cout<<"Enter the number of positive and negative points(in order) for this question"<<endl;
            cin>>positivePoints>>negativePoints;
            new_question.set_points(positivePoints, negativePoints);
        }
        whole_grade += positivePoints;
        questionlist.push_back(new_question);
        cout<<"Question was added succesfully!"<<endl;
        return;
    }while(qtype != "x");
    return;
}
void Exam::Remove_question(){
    int index;
    cout<<"Enter the index of the question you want removed"<<endl;
    cin>>index;
    auto question = questionlist.begin();
    advance(question, index-1);
    whole_grade -= question->points_positive;
    questionlist.erase(question);   
}

json Exam::ExamtoJson(){
    json j;
    j["name"] = name;
    j["code"] = code;
    j["creator's id"] = creator_id;
    j["whole grade"] = whole_grade;
    j["questions"] = json::array();
    for(const auto& question : questionlist){
        j["questions"].push_back(question.Questions_toJson());
    }
    return j;
}
json teacher::toJson(){
    return {
        {"firstname", FirstName},
        {"lastname", LastName},
        {"id", id},
        {"occupation", occupation},
        {"password", password},
        {"courses",courses}
    };
}
json Question::Questions_toJson() const {
    json j;
    j["type"] = type;
    j["text"] = text;
    j["answer"] = answer;
    if(type == 1){
        j["choices"] = choices;
    }
    else j["choices"] = json::array();
    j["positive points"] = points_positive;
    j["negative points"] = points_negative;
    j["answer status"] = answer_status;
    j["given answers"] = json::array();
    return j;
}
void teacher::view_Exams(){//press 1 to see exam details 2 to get the grades list
    cout<<"Choose an exam"<<endl;
    ifstream examin("exams.json");
    if(!examin.good()){
        cout<<"error while opening exam file!"<<endl;
        return;
    }
    json exam_data;
    examin >> exam_data;
    int index = 1, chosen_index = 0, command;
    vector<string> displayed_codes;
    for(const auto& exam : exam_data["exams"]){
        if(exam["creator's id"] == this->id){
        cout<<index++<<". "<<exam["name"]<<" "<<exam["code"]<<endl;
        displayed_codes.push_back(exam["code"].get<string>());
        }
    }
    if(displayed_codes.empty()){
        cout << "You don't have any exams yet."<< endl;
    }
    do{
        cout<<"Enter a valid index"<<endl;
        cin>>chosen_index;
    }while(chosen_index <= 0 || chosen_index >= index);
    cout<<"Press 1 to see the details. Press 2 to get the grades list."<<endl;
    cin>>command;
    if(command == 1) show_exam(exam_data, displayed_codes[chosen_index - 1]);
    else if(command == 2) showGradeList(displayed_codes[chosen_index - 1]);
    examin.close();
    return;
}
void teacher::showGradeList(string exam_code){
    ifstream resultsFile("resaults.json");
    json resultsData;
    if (resultsFile.good()) {
        resultsFile >> resultsData;
    } else {
        cout << "No results data found!\n";
        return;
    }
    vector<float> allgrades;
    vector<tuple<string, string, float>> allRequiredData;
    //vector<StudentGrades> allRequiredData;
    float sumofAllgrades = 0;
    int total_count = 0;
    float MaxGrade = 0, Average = 0;
    bool Examexists = false;
    for(const auto& data : resultsData["resaults"]){
        if(data["exam code"] == exam_code){
            Examexists = true;
            sumofAllgrades += data["grade"].get<float>();
            total_count++;
            string fullname = data["Students Name"].get<string>();
            string sid = data["studentID"].get<string>();
            float grade = data["grade"].get<float>();
            allgrades.push_back(grade);
            allRequiredData.emplace_back(fullname, sid, grade);
            cout << fullname << sid << grade << endl;
        }
    }
    //sort(allRequiredData.grade.begin(), allRequiredData.grade.end(), [](auto& a, auto& b){return a.grade > b.grade})
    sort(allRequiredData.begin(), allRequiredData.end(), 
        [](auto& a, auto& b) { return get<2>(a) > get<2>(b); });
    sort(allgrades.begin(), allgrades.end(),
        [](auto& a, auto& b) { return a > b; });
        
    MaxGrade = *allgrades.begin();
    cout<<MaxGrade<<endl;
    Average = sumofAllgrades / total_count;
    if(!Examexists){
        cout<<"Exam not found!"<<endl;
        return;
    }
    ofstream examresults("exam_results_" + exam_code + ".txt");
    if (!examresults) {
        cerr << "Error: Could not create report file\n";
        return;
    }
    int index = 1;
    examresults<<"Exam Results"<<endl<<"Exam Code : "<<exam_code<<endl;
    for(const auto& data : resultsData["resaults"]){
        if(data["exam code"] == exam_code){
            for(auto& i : allgrades){
                if(i == data["grade"].get<float>()){
                    examresults<<index++<<". "<<"Name : "<<data["Students Name"].get<string>()
                               <<"    id : "<<data["studentID"].get<string>()
                               <<"    grade : "<<i<<endl;
                }
            }
        }
    }
    examresults<<endl<<"Maximum Grade : "<<MaxGrade<<"      Average Grade : "<<Average<<endl;
    examresults.close();
    cout<<"Exam Result saved as exam_results_"<<exam_code<<".txt"<<endl;
    return;

}
void teacher::show_exam(json& exam_data, string exam_code){
    for(const auto& exam : exam_data["exams"]){
        if(exam["code"] == exam_code){
            cout<<"Exam name : "<<exam["name"]<<endl<<"Exam code : "<<exam["code"]<<endl;
            int question_index = 1;
            for(const auto& question : exam["questions"]){
                cout<<question_index++<<". "<<question["text"];
                if(question["type"] == 1) cout<<question["choices"];
                cout<<endl<<"Positive Points : "<<question["positive points"]<<"    negative points"<<question["negative points"];
                cout<<endl;
                return;
            }
        }
    }
}


//important *
string Exam::code_generator(){
    long int x;
    string c;
    do{
        x = 1000000000 + (rand()%9000000000);
        c = to_string(x);
    }while(find(all_codes.begin(), all_codes.end(), c) != all_codes.end());
    code = c;
    return code;
    all_codes.push_back(c);
}

float Exam::calculate_grade(){
    int question_count = 0;
    int count_correct_answers = 0, count_wrong_answers = 0;
    int grade = 0, whole_grade;
    for(auto i : questionlist){
        question_count++;
        whole_grade += i.points_positive;
        if(i.answer_status == 1){
            grade += i.points_negative;
            count_correct_answers++;
        }
        if(i.answer_status == 3){
            count_wrong_answers++;
            grade -= i.points_negative;
        }
    }
    //grade = 20 * (count_correct_answers / question_count);
    return grade;
}


void student::student_panel(){
    cout<<"1.Add exam"<<endl<<"2.View exams"<<endl<<"3.Participate in exam"<<endl<<"Press x to return to the main page"<<endl;
    do{
        getline(cin, command);
        if(command == "1"){
            add_exam();
        }
        if(command == "2"){
            view_Exams();
        }
        if(command == "3"){
            take_Exam();
        }
    }while(command != "x");
    return;

}

void teacher::set_courses(string c) { courses.push_back(c); }


json student::toJson(){
    return {
        {"firstname", FirstName},
        {"lastname", LastName},
        {"id", id},
        {"occupation", occupation},
        {"password", password},
        {"major",major}
    };
}

void student::add_exam(){//handle exam codes that are added multipe times
    string exam_code;
    cout<<"Enter exam code : ";
    getline(cin , exam_code);
    ifstream examfile("exams.json");
    ifstream userfile("users.json");
    if(!examfile.good() || !userfile.good()) return;
    json exam_data, user_data;
    examfile >> exam_data;
    userfile >> user_data;
    for(auto check : exam_codes){
        if(exam_code == check){
            cout<<"You have already added this exam!"<<endl;
            return;
        }
    }
    bool exam_exists = false;
    if (exam_data.contains("code") && exam_data["code"].is_array()) {
        for (const auto& c : exam_data["code"]) {
            if (c == exam_code) { exam_exists = true; break; }
        }
    } else if (exam_data.contains("exams") && exam_data["exams"].is_array()) {
        for (const auto& e : exam_data["exams"]) {
            if (e.contains("code") && e["code"] == exam_code) { exam_exists = true; break; }
        }
    }
    if (!exam_exists) { cout << "Exam not found!\n"; return; }
    bool user_found = false;
    for (auto& s : user_data["users"]) {
        if (s["id"] == id) {
            if (!s.contains("ExamCodes") || !s["ExamCodes"].is_array())
                s["ExamCodes"] = json::array();
            s["ExamCodes"].push_back(exam_code);
            user_found = true;
            break;
        }
    }
    if (!user_found) { cout << "User not found!\n"; return; }

    exam_codes.push_back(exam_code);
    cout << "Exam added successfully!\n";
    ofstream outfile("users.json", ios::trunc);
    outfile << user_data.dump(4);
}

void student::view_Exams(){
    ifstream examsFile("exams.json");
    ifstream usersFile("users.json");
    json exam_data, user_data;
    examsFile >> exam_data;
    usersFile >> user_data;
    vector<string> codes;
    for (const auto& u : user_data["users"]) {
        if (u["id"] == id && u.contains("ExamCodes") && u["ExamCodes"].is_array()) {
            for (const auto& c : u["ExamCodes"]) codes.push_back(c.get<string>());
            break;
        }
    }
    int index = 1;
    for(auto& i : codes){
        for(auto& exam : exam_data["exams"]){
            if(exam["code"] == i){
                cout<<index++<<". "<<exam["name"]<<" "<<i<<endl;
            }
        }
    }
    cout<<"Enter an exam code to see the report card. Press x to exit"<<endl;
    getline(cin, command);
    if(command != "x"){
        showStudentResault(command);
    }
    else{
        return;
    }
}
void student::take_Exam(){
    fstream examsFile("exams.json");
    json exam_data;
    examsFile >> exam_data;
    float grade = 0;
    string given_code, given_answer;
    cout<<"Enter the code to participate in that exam"<<endl;
    getline(cin,given_code);
    for(auto& exam : exam_data["exams"]){
        if(exam["code"] == given_code){
            for(auto& question : exam["questions"]){
                cout<<question["text"]<<endl;
                vector<string> choices_display = question["choices"];
                vector<string> choices;
                if(question["type"] == 1){
                    //vector<string> choices_display = question["choices"];
                    
                    int x = rand()%4, y = rand()%3, z = rand()%2;
                    cout<<"1."<<choices_display[x];
                    choices.push_back(choices_display[x]);
                    choices_display.erase(choices_display.begin()+x);
                    cout<<"    2."<<choices_display[y];
                    choices.push_back(choices_display[y]);
                    choices_display.erase(choices_display.begin()+y);
                    cout<<"    3."<<choices_display[z];
                    choices.push_back(choices_display[z]);
                    choices_display.erase(choices_display.begin()+z);
                    cout<<"    4."<<choices_display[0]<<endl;
                    choices.push_back(choices_display[0]);
                }
                getline(cin, given_answer);//handle capital letters
                //question.setdefault("given answers", []);
                if (!question.contains("given answers") || !question["given answers"].is_array())
                    question["given answers"] = json::array();
                if(question["type"] == 1) question["given answers"].push_back(json{
                    {"student_id", id},
                    {"answer", choices[stoi(given_answer) - 1]}
                });
                else{
                    question["given answers"].push_back(json{
                        {"student_id", id},
                        {"answer", given_answer}
                    });
                }
            }
            
            //exam_checker(exam_data, given_code);
            cout<<"Your grade is : ";
            cout<<setprecision(2)<<fixed<<exam_checker(exam_data, given_code)<<endl;
            examsFile.close();
            ofstream out("exams.json");
            out << exam_data.dump(4);

            return;
        }
    }
    cout<<"Exam not found!"<<endl;
    return;
}
float student::exam_checker(json& exam_data, string exam_code){
    //int correctMCQcount = 0, incorrectMSQcount = 0, correctSAcount = 0, incorrectSAcount = 0;
    float MCQgrade = 0, SAgrade = 0, total_grade = 0;
    vector<string> checked_answers;
    for(const auto& exam : exam_data["exams"]){
        if(exam["code"] == exam_code){
            for(const auto& q : exam["questions"]){
                string student_answer = "";
                for(auto& entry : q["given answers"]){
                    if(entry["student_id"] == id){
                        student_answer = entry["answer"];
                        break;
                    }
                }
                if (student_answer.empty()) continue;
                if (q["answer"] == student_answer){
                    if(q["type"] == 1){
                        MCQgrade = MCQgrade + q["positive points"].get<float>();
                        total_grade = total_grade + q["positive points"].get<float>();
                        checked_answers.push_back("correct");
                    }
                    else if(q["type"] == 3){
                        SAgrade += q["positive points"].get<float>();
                        total_grade = total_grade + q["positive points"].get<float>();
                        checked_answers.push_back("correct");
                    }
                    else{
                        total_grade = total_grade + q["positive points"].get<float>();
                    }
                } else{
                    if(q["type"] == 1){
                        MCQgrade = MCQgrade - q["negative points"].get<float>();
                        total_grade = total_grade - q["negative points"].get<float>();
                        checked_answers.push_back("incorrect "+q["answer"].get<string>());
                    }
                    else if(q["type"] == 3){
                        SAgrade -= q["negative points"].get<float>();
                        total_grade = total_grade - q["negative points"].get<float>();
                        checked_answers.push_back("incorrect "+q["answer"].get<string>());
                    }
                    else{
                        checked_answers.push_back("incorrect "+q["answer"].get<string>());
                    }
                }
            }
            string Fullname = FirstName + LastName;
            Resault(exam_code, id, Fullname, total_grade, MCQgrade, SAgrade, checked_answers).saveStudentResaults();
            return total_grade;
        }
    }
}
json Resault::toJson(){
    return{
        {"exam code",exam_code},
        {"studentID",student_id},
        {"Students Name",students_name},
        {"grade",grade},
        {"Multipe choice questions points",gradeOfMCQs},
        {"Short answer points",gradeOfShortAnswers},
        {"Checked answers",checked_answers}
    };
};
// void Resault:: saveExamResaults(){

// };
void Resault::saveStudentResaults(){
    ifstream inFile("resaults.json");
    json j;
    if(inFile.good()) inFile >> j;
    inFile.close();
    j["resaults"].push_back(toJson());
    ofstream outFile("resaults.json");
    outFile << j.dump(4);
};
void student:: showStudentResault(string exam_code){
    ifstream resultsFile("resaults.json");
    json resultsData;
    if (resultsFile.good()) {
        resultsFile >> resultsData;
    } else {
        cout << "No results data found!\n";
        return;
    }
    vector<float> allgrades;
    vector<string> checked_answers;
    float sumofAllgrades = 0, gradeOfMCQs = 0,gradeOfShortAnswers = 0 ;
    int rank = 1, total_count = 0;
    float MyGrade = -1, MaxGrade = 0, Average = 0;
    bool Examexists = false;
    for(const auto& data : resultsData["resaults"]){
        if(data["exam code"] == exam_code){
            Examexists = true;
            sumofAllgrades += data["grade"].get<float>();
            total_count++;
            allgrades.push_back(data["grade"]);
            if(data["studentID"] == id){
                MyGrade = data["grade"];
                MaxGrade = MyGrade;
                gradeOfMCQs = data["Multipe choice questions points"];
                gradeOfShortAnswers = data["Short answer points"];
                checked_answers = data["Checked answers"];
            }
        }
    }
    Average = sumofAllgrades / total_count;
    if(!Examexists){
        cout<<"Exam not found!"<<endl;
        return;
    }
    if(MyGrade == -1){
        cout<<"You have yet to take this exam"<<endl;
        return;
    }
    for(const auto& i : allgrades){
        if(i > MyGrade){
            MaxGrade = i;
            rank++;
        }
    }
    ofstream report("report_card_" + id + "_" + exam_code + ".txt");
    if (!report) {
        cerr << "Error: Could not create report file\n";
        return;
    }
    report<<"Exam code : "<<exam_code<<endl 
          <<"Your Total Grade : "<<MyGrade<<endl
          <<"Your rank so far :"<<rank<<" of "<<total_count<<endl
          <<"Your Grade for short Answers : "<<gradeOfShortAnswers<<endl
          <<"Your Grade for Multiple Choice Questions : "<<gradeOfMCQs<<endl
          <<"Overview of your answers : "<<endl;
          int index = 1;
          for(const auto& i : checked_answers){
            report<<index++<<". "<<i<<endl;
          }
    report<<"Maximum Grade so far : "<<MaxGrade<<endl
          <<"Average Grade : "<<Average<<endl;
        report.close();
        cout<<"Report card saved as report_card_"<<id<<"_"<<exam_code<<".txt"<<endl;
        return;
}

vector<string> Exam:: all_codes;
