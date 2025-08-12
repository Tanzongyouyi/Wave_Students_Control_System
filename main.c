#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

// 常量定义
#define MAX_NAME_LEN 50
#define MAX_ID_LEN 20
#define MAX_PASSWORD_LEN 20
#define MAX_COURSE_NAME_LEN 50
#define MAX_DEPARTMENT_LEN 50
#define MAX_ADDRESS_LEN 100
#define MAX_PHONE_LEN 15
#define MAX_EMAIL_LEN 50
#define MAX_STUDENTS 1000
#define MAX_COURSES 100
#define MAX_ENROLLMENTS 2000
#define MAX_TEACHERS 100
#define MAX_CLASSES 50
#define MAX_GRADES 5
#define HASH_TABLE_SIZE 1009
#define FILENAME "student_management.dat"

// 枚举类型定义
typedef enum {
    MALE,
    FEMALE,
    OTHER
} Gender;

typedef enum {
    FRESHMAN,
    SOPHOMORE,
    JUNIOR,
    SENIOR,
    GRADUATE
} Year;

typedef enum {
    ADMIN,
    TEACHER,
    STUDENT,
    GUEST
} UserRole;

typedef enum {
    A,
    B,
    C,
    D,
    F
} Grade;

typedef enum {
    SPRING,
    SUMMER,
    FALL,
    WINTER
} Semester;

// 结构体定义
typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char course_id[MAX_ID_LEN];
    char name[MAX_COURSE_NAME_LEN];
    int credits;
    char department[MAX_DEPARTMENT_LEN];
    char teacher_id[MAX_ID_LEN];
    Semester semester;
    int year;
    int max_students;
    int current_students;
} Course;

typedef struct {
    char student_id[MAX_ID_LEN];
    char course_id[MAX_ID_LEN];
    Grade grades[MAX_GRADES];
    float average;
    Date enrollment_date;
    bool completed;
} Enrollment;

typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char password[MAX_PASSWORD_LEN];
    Gender gender;
    Date birth_date;
    char address[MAX_ADDRESS_LEN];
    char phone[MAX_PHONE_LEN];
    char email[MAX_EMAIL_LEN];
    Year year;
    char department[MAX_DEPARTMENT_LEN];
    float gpa;
    int total_credits;
    Date admission_date;
    bool is_active;
    char class_id[MAX_ID_LEN];
} Student;

typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char password[MAX_PASSWORD_LEN];
    Gender gender;
    Date birth_date;
    char department[MAX_DEPARTMENT_LEN];
    char phone[MAX_PHONE_LEN];
    char email[MAX_EMAIL_LEN];
    Date hire_date;
    char title[MAX_NAME_LEN];
    float salary;
    bool is_active;
} Teacher;

typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    char department[MAX_DEPARTMENT_LEN];
    char teacher_id[MAX_ID_LEN];
    int student_count;
    int max_students;
} Class;

typedef struct {
    char username[MAX_ID_LEN];
    char password[MAX_PASSWORD_LEN];
    UserRole role;
    bool is_logged_in;
    time_t last_login;
    time_t session_timeout;
} UserSession;

typedef struct HashNode {
    char key[MAX_ID_LEN];
    void *data;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode *table[HASH_TABLE_SIZE];
    int size;
} HashTable;

// 全局变量
Student students[MAX_STUDENTS];
Course courses[MAX_COURSES];
Enrollment enrollments[MAX_ENROLLMENTS];
Teacher teachers[MAX_TEACHERS];
Class classes[MAX_CLASSES];
UserSession current_session;
HashTable student_hash, course_hash, teacher_hash, class_hash;

int student_count = 0;
int course_count = 0;
int enrollment_count = 0;
int teacher_count = 0;
int class_count = 0;

// 函数声明
void initialize_system();
void main_menu();
void login();
void logout();
void admin_menu();
void teacher_menu();
void student_menu();
void guest_menu();

// 学生管理函数
void add_student();
void edit_student();
void delete_student();
void view_student();
void list_all_students();
void search_student();
void sort_students();

// 课程管理函数
void add_course();
void edit_course();
void delete_course();
void view_course();
void list_all_courses();
void search_course();
void sort_courses();

// 选课管理函数
void enroll_student();
void drop_course();
void view_enrollments();
void assign_grade();
void calculate_gpa();

// 教师管理函数
void add_teacher();
void edit_teacher();
void delete_teacher();
void view_teacher();
void list_all_teachers();
void search_teacher();
void sort_teachers();

// 班级管理函数
void add_class();
void edit_class();
void delete_class();
void view_class();
void list_all_classes();
void assign_teacher_to_class();
void assign_student_to_class();

// 统计和报告函数
void generate_student_report();
void generate_course_report();
void generate_teacher_report();
void generate_department_report();
void generate_statistics();

// 系统管理函数
void backup_data();
void restore_data();
void change_password();
void system_settings();
void view_audit_log();

// 辅助函数
void clear_input_buffer();
int get_int_input(const char *prompt);
float get_float_input(const char *prompt);
void get_string_input(const char *prompt, char *buffer, int max_len);
Date get_date_input(const char *prompt);
Gender get_gender_input(const char *prompt);
Year get_year_input(const char *prompt);
Grade get_grade_input(const char *prompt);
Semester get_semester_input(const char *prompt);
UserRole get_role_input(const char *prompt);
bool is_valid_email(const char *email);
bool is_valid_phone(const char *phone);
bool is_valid_id(const char *id);
void to_lower_case(char *str);
void to_upper_case(char *str);
void trim_whitespace(char *str);

// 哈希表函数
unsigned int hash_function(const char *key);
void init_hash_table(HashTable *ht);
void insert_hash(HashTable *ht, const char *key, void *data);
void *lookup_hash(HashTable *ht, const char *key);
void remove_hash(HashTable *ht, const char *key);
void free_hash_table(HashTable *ht);

// 文件操作函数
void save_data();
void load_data();
void write_string(FILE *file, const char *str);
void read_string(FILE *file, char *buffer, int max_len);

// 排序函数
int compare_students_by_name(const void *a, const void *b);
int compare_students_by_id(const void *a, const void *b);
int compare_students_by_gpa(const void *a, const void *b);
int compare_courses_by_name(const void *a, const void *b);
int compare_courses_by_id(const void *a, const void *b);
int compare_courses_by_credits(const void *a, const void *b);
int compare_teachers_by_name(const void *a, const void *b);
int compare_teachers_by_id(const void *a, const void *b);
int compare_teachers_by_salary(const void *a, const void *b);

// 主函数
int main() {
    initialize_system();
    load_data();
    
    printf("\n=== 超级复杂的学生管理系统 ===\n");
    printf("系统初始化完成，数据已加载。\n");
    
    main_menu();
    
    save_data();
    printf("\n系统数据已保存，感谢使用！\n");
    return 0;
}

// 初始化系统
void initialize_system() {
    // 初始化哈希表
    init_hash_table(&student_hash);
    init_hash_table(&course_hash);
    init_hash_table(&teacher_hash);
    init_hash_table(&class_hash);
    
    // 初始化会话
    memset(&current_session, 0, sizeof(UserSession));
    current_session.role = GUEST;
    current_session.is_logged_in = false;
    
    // 设置随机种子
    srand(time(NULL));
    
    // 添加一些默认数据
    if (student_count == 0) {
        // 添加默认管理员
        strcpy(students[0].id, "admin");
        strcpy(students[0].name, "系统管理员");
        strcpy(students[0].password, "admin123");
        students[0].gender = OTHER;
        students[0].birth_date = (Date){1, 1, 1980};
        strcpy(students[0].address, "系统管理中心");
        strcpy(students[0].phone, "00000000000");
        strcpy(students[0].email, "admin@system.com");
        students[0].year = GRADUATE;
        strcpy(students[0].department, "系统管理");
        students[0].gpa = 4.0;
        students[0].total_credits = 0;
        students[0].admission_date = (Date){1, 1, 2000};
        students[0].is_active = true;
        student_count = 1;
        
        // 添加默认教师
        strcpy(teachers[0].id, "teacher1");
        strcpy(teachers[0].name, "张教授");
        strcpy(teachers[0].password, "teacher123");
        teachers[0].gender = MALE;
        teachers[0].birth_date = (Date){15, 5, 1975};
        strcpy(teachers[0].department, "计算机科学");
        strcpy(teachers[0].phone, "13800138000");
        strcpy(teachers[0].email, "zhang@university.edu");
        teachers[0].hire_date = (Date){1, 9, 2000};
        strcpy(teachers[0].title, "教授");
        teachers[0].salary = 15000.0;
        teachers[0].is_active = true;
        teacher_count = 1;
        
        // 添加默认课程
        strcpy(courses[0].course_id, "CS101");
        strcpy(courses[0].name, "计算机科学导论");
        courses[0].credits = 3;
        strcpy(courses[0].department, "计算机科学");
        strcpy(courses[0].teacher_id, "teacher1");
        courses[0].semester = FALL;
        courses[0].year = 2023;
        courses[0].max_students = 100;
        courses[0].current_students = 0;
        course_count = 1;
        
        // 添加默认班级
        strcpy(classes[0].id, "CS101A");
        strcpy(classes[0].name, "计算机科学1班");
        strcpy(classes[0].department, "计算机科学");
        strcpy(classes[0].teacher_id, "teacher1");
        classes[0].student_count = 0;
        classes[0].max_students = 50;
        class_count = 1;
    }
}

// 主菜单
void main_menu() {
    int choice;
    
    while (1) {
        printf("\n=== 主菜单 ===\n");
        printf("1. 登录系统\n");
        printf("2. 访客模式\n");
        printf("3. 退出系统\n");
        printf("请选择操作: ");
        
        choice = get_int_input("");
        
        switch (choice) {
            case 1:
                login();
                if (current_session.is_logged_in) {
                    switch (current_session.role) {
                        case ADMIN:
                            admin_menu();
                            break;
                        case TEACHER:
                            teacher_menu();
                            break;
                        case STUDENT:
                            student_menu();
                            break;
                        default:
                            guest_menu();
                    }
                }
                break;
            case 2:
                guest_menu();
                break;
            case 3:
                return;
            default:
                printf("无效选择，请重新输入。\n");
        }
    }
}

// 登录功能
void login() {
    char username[MAX_ID_LEN];
    char password[MAX_PASSWORD_LEN];
    int attempts = 3;
    
    while (attempts > 0) {
        printf("\n=== 系统登录 ===\n");
        printf("用户名: ");
        get_string_input("", username, MAX_ID_LEN);
        printf("密码: ");
        get_string_input("", password, MAX_PASSWORD_LEN);
        
        // 检查管理员
        if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
            strcpy(current_session.username, username);
            strcpy(current_session.password, password);
            current_session.role = ADMIN;
            current_session.is_logged_in = true;
            current_session.last_login = time(NULL);
            current_session.session_timeout = time(NULL) + 1800; // 30分钟超时
            printf("登录成功！欢迎管理员。\n");
            return;
        }
        
        // 检查教师
        for (int i = 0; i < teacher_count; i++) {
            if (strcmp(teachers[i].id, username) == 0 && 
                strcmp(teachers[i].password, password) == 0 && 
                teachers[i].is_active) {
                strcpy(current_session.username, username);
                strcpy(current_session.password, password);
                current_session.role = TEACHER;
                current_session.is_logged_in = true;
                current_session.last_login = time(NULL);
                current_session.session_timeout = time(NULL) + 1800;
                printf("登录成功！欢迎 %s 老师。\n", teachers[i].name);
                return;
            }
        }
        
        // 检查学生
        for (int i = 0; i < student_count; i++) {
            if (strcmp(students[i].id, username) == 0 && 
                strcmp(students[i].password, password) == 0 && 
                students[i].is_active) {
                strcpy(current_session.username, username);
                strcpy(current_session.password, password);
                current_session.role = STUDENT;
                current_session.is_logged_in = true;
                current_session.last_login = time(NULL);
                current_session.session_timeout = time(NULL) + 1800;
                printf("登录成功！欢迎 %s 同学。\n", students[i].name);
                return;
            }
        }
        
        attempts--;
        printf("用户名或密码错误，还剩 %d 次尝试机会。\n", attempts);
    }
    
    printf("登录失败，请稍后再试。\n");
}

// 登出功能
void logout() {
    if (current_session.is_logged_in) {
        printf("用户 %s 已登出。\n", current_session.username);
        memset(&current_session, 0, sizeof(UserSession));
        current_session.role = GUEST;
        current_session.is_logged_in = false;
    }
}

// 管理员菜单
void admin_menu() {
    int choice;
    
    while (1) {
        // 检查会话是否超时
        if (time(NULL) > current_session.session_timeout) {
            printf("会话已超时，请重新登录。\n");
            logout();
            return;
        }
        
        printf("\n=== 管理员菜单 ===\n");
        printf("1. 学生管理\n");
        printf("2. 课程管理\n");
        printf("3. 教师管理\n");
        printf("4. 班级管理\n");
        printf("5. 选课管理\n");
        printf("6. 统计报告\n");
        printf("7. 系统管理\n");
        printf("8. 修改密码\n");
        printf("9. 登出\n");
        printf("请选择操作: ");
        
        choice = get_int_input("");
        
        switch (choice) {
            case 1: {
                int sub_choice;
                printf("\n=== 学生管理 ===\n");
                printf("1. 添加学生\n");
                printf("2. 编辑学生\n");
                printf("3. 删除学生\n");
                printf("4. 查看学生\n");
                printf("5. 列出所有学生\n");
                printf("6. 搜索学生\n");
                printf("7. 排序学生\n");
                printf("8. 返回\n");
                printf("请选择操作: ");
                sub_choice = get_int_input("");
                
                switch (sub_choice) {
                    case 1: add_student(); break;
                    case 2: edit_student(); break;
                    case 3: delete_student(); break;
                    case 4: view_student(); break;
                    case 5: list_all_students(); break;
                    case 6: search_student(); break;
                    case 7: sort_students(); break;
                    case 8: break;
                    default: printf("无效选择。\n");
                }
                break;
            }
            case 2: {
                int sub_choice;
                printf("\n=== 课程管理 ===\n");
                printf("1. 添加课程\n");
                printf("2. 编辑课程\n");
                printf("3. 删除课程\n");
                printf("4. 查看课程\n");
                printf("5. 列出所有课程\n");
                printf("6. 搜索课程\n");
                printf("7. 排序课程\n");
                printf("8. 返回\n");
                printf("请选择操作: ");
                sub_choice = get_int_input("");
                
                switch (sub_choice) {
                    case 1: add_course(); break;
                    case 2: edit_course(); break;
                    case 3: delete_course(); break;
                    case 4: view_course(); break;
                    case 5: list_all_courses(); break;
                    case 6: search_course(); break;
                    case 7: sort_courses(); break;
                    case 8: break;
                    default: printf("无效选择。\n");
                }
                break;
            }
            case 3: {
                int sub_choice;
                printf("\n=== 教师管理 ===\n");
                printf("1. 添加教师\n");
                printf("2. 编辑教师\n");
                printf("3. 删除教师\n");
                printf("4. 查看教师\n");
                printf("5. 列出所有教师\n");
                printf("6. 搜索教师\n");
                printf("7. 排序教师\n");
                printf("8. 返回\n");
                printf("请选择操作: ");
                sub_choice = get_int_input("");
                
                switch (sub_choice) {
                    case 1: add_teacher(); break;
                    case 2: edit_teacher(); break;
                    case 3: delete_teacher(); break;
                    case 4: view_teacher(); break;
                    case 5: list_all_teachers(); break;
                    case 6: search_teacher(); break;
                    case 7: sort_teachers(); break;
                    case 8: break;
                    default: printf("无效选择。\n");
                }
                break;
            }
            case 4: {
                int sub_choice;
                printf("\n=== 班级管理 ===\n");
                printf("1. 添加班级\n");
                printf("2. 编辑班级\n");
                printf("3. 删除班级\n");
                printf("4. 查看班级\n");
                printf("5. 列出所有班级\n");
                printf("6. 分配教师到班级\n");
                printf("7. 分配学生到班级\n");
                printf("8. 返回\n");
                printf("请选择操作: ");
                sub_choice = get_int_input("");
                
                switch (sub_choice) {
                    case 1: add_class(); break;
                    case 2: edit_class(); break;
                    case 3: delete_class(); break;
                    case 4: view_class(); break;
                    case 5: list_all_classes(); break;
                    case 6: assign_teacher_to_class(); break;
                    case 7: assign_student_to_class(); break;
                    case 8: break;
                    default: printf("无效选择。\n");
                }
                break;
            }
            case 5: {
                int sub_choice;
                printf("\n=== 选课管理 ===\n");
                printf("1. 学生选课\n");
                printf("2. 学生退课\n");
                printf("3. 查看选课记录\n");
                printf("4. 分配成绩\n");
                printf("5. 计算GPA\n");
                printf("6. 返回\n");
                printf("请选择操作: ");
                sub_choice = get_int_input("");
                
                switch (sub_choice) {
                    case 1: enroll_student(); break;
                    case 2: drop_course(); break;
                    case 3: view_enrollments(); break;
                    case 4: assign_grade(); break;
                    case 5: calculate_gpa(); break;
                    case 6: break;
                    default: printf("无效选择。\n");
                }
                break;
            }
            case 6: {
                int sub_choice;
                printf("\n=== 统计报告 ===\n");
                printf("1. 生成学生报告\n");
                printf("2. 生成课程报告\n");
                printf("3. 生成教师报告\n");
                printf("4. 生成院系报告\n");
                printf("5. 生成统计信息\n");
                printf("6. 返回\n");
                printf("请选择操作: ");
                sub_choice = get_int_input("");
                
                switch (sub_choice) {
                    case 1: generate_student_report(); break;
                    case 2: generate_course_report(); break;
                    case 3: generate_teacher_report(); break;
                    case 4: generate_department_report(); break;
                    case 5: generate_statistics(); break;
                    case 6: break;
                    default: printf("无效选择。\n");
                }
                break;
            }
            case 7: {
                int sub_choice;
                printf("\n=== 系统管理 ===\n");
                printf("1. 备份数据\n");
                printf("2. 恢复数据\n");
                printf("3. 系统设置\n");
                printf("4. 查看审计日志\n");
                printf("5. 返回\n");
                printf("请选择操作: ");
                sub_choice = get_int_input("");
                
                switch (sub_choice) {
                    case 1: backup_data(); break;
                    case 2: restore_data(); break;
                    case 3: system_settings(); break;
                    case 4: view_audit_log(); break;
                    case 5: break;
                    default: printf("无效选择。\n");
                }
                break;
            }
            case 8:
                change_password();
                break;
            case 9:
                logout();
                return;
            default:
                printf("无效选择，请重新输入。\n");
        }
    }
}

// 教师菜单
void teacher_menu() {
    int choice;
    
    while (1) {
        // 检查会话是否超时
        if (time(NULL) > current_session.session_timeout) {
            printf("会话已超时，请重新登录。\n");
            logout();
            return;
        }
        
        printf("\n=== 教师菜单 ===\n");
        printf("1. 查看个人信息\n");
        printf("2. 修改个人信息\n");
        printf("3. 查看所教课程\n");
        printf("4. 查看课程学生\n");
        printf("5. 分配成绩\n");
        printf("6. 生成课程报告\n");
        printf("7. 修改密码\n");
        printf("8. 登出\n");
        printf("请选择操作: ");
        
        choice = get_int_input("");
        
        switch (choice) {
            case 1: {
                // 查看教师个人信息
                Teacher *teacher = NULL;
                for (int i = 0; i < teacher_count; i++) {
                    if (strcmp(teachers[i].id, current_session.username) == 0) {
                        teacher = &teachers[i];
                        break;
                    }
                }
                
                if (teacher) {
                    printf("\n=== 教师信息 ===\n");
                    printf("ID: %s\n", teacher->id);
                    printf("姓名: %s\n", teacher->name);
                    printf("性别: %s\n", teacher->gender == MALE ? "男" : 
                           teacher->gender == FEMALE ? "女" : "其他");
                    printf("出生日期: %d-%d-%d\n", teacher->birth_date.year, 
                           teacher->birth_date.month, teacher->birth_date.day);
                    printf("院系: %s\n", teacher->department);
                    printf("电话: %s\n", teacher->phone);
                    printf("邮箱: %s\n", teacher->email);
                    printf("聘用日期: %d-%d-%d\n", teacher->hire_date.year, 
                           teacher->hire_date.month, teacher->hire_date.day);
                    printf("职称: %s\n", teacher->title);
                    printf("薪资: %.2f\n", teacher->salary);
                    printf("状态: %s\n", teacher->is_active ? "在职" : "离职");
                } else {
                    printf("未找到教师信息。\n");
                }
                break;
            }
            case 2: {
                // 修改教师个人信息
                Teacher *teacher = NULL;
                for (int i = 0; i < teacher_count; i++) {
                    if (strcmp(teachers[i].id, current_session.username) == 0) {
                        teacher = &teachers[i];
                        break;
                    }
                }
                
                if (teacher) {
                    printf("\n=== 修改教师信息 ===\n");
                    printf("当前姓名: %s\n", teacher->name);
                    printf("新姓名 (留空保持不变): ");
                    char new_name[MAX_NAME_LEN];
                    get_string_input("", new_name, MAX_NAME_LEN);
                    if (strlen(new_name) > 0) {
                        strcpy(teacher->name, new_name);
                    }
                    
                    printf("当前性别: %s\n", teacher->gender == MALE ? "男" : 
                           teacher->gender == FEMALE ? "女" : "其他");
                    printf("新性别 (0-男, 1-女, 2-其他, 留空保持不变): ");
                    char gender_input[10];
                    get_string_input("", gender_input, 10);
                    if (strlen(gender_input) > 0) {
                        int gender_val = atoi(gender_input);
                        if (gender_val >= 0 && gender_val <= 2) {
                            teacher->gender = (Gender)gender_val;
                        }
                    }
                    
                    printf("当前电话: %s\n", teacher->phone);
                    printf("新电话 (留空保持不变): ");
                    char new_phone[MAX_PHONE_LEN];
                    get_string_input("", new_phone, MAX_PHONE_LEN);
                    if (strlen(new_phone) > 0) {
                        if (is_valid_phone(new_phone)) {
                            strcpy(teacher->phone, new_phone);
                        } else {
                            printf("无效的电话号码。\n");
                        }
                    }
                    
                    printf("当前邮箱: %s\n", teacher->email);
                    printf("新邮箱 (留空保持不变): ");
                    char new_email[MAX_EMAIL_LEN];
                    get_string_input("", new_email, MAX_EMAIL_LEN);
                    if (strlen(new_email) > 0) {
                        if (is_valid_email(new_email)) {
                            strcpy(teacher->email, new_email);
                        } else {
                            printf("无效的邮箱地址。\n");
                        }
                    }
                    
                    printf("信息已更新。\n");
                } else {
                    printf("未找到教师信息。\n");
                }
                break;
            }
            case 3: {
                // 查看所教课程
                printf("\n=== 所教课程 ===\n");
                bool found = false;
                
                for (int i = 0; i < course_count; i++) {
                    if (strcmp(courses[i].teacher_id, current_session.username) == 0) {
                        printf("课程ID: %s\n", courses[i].course_id);
                        printf("课程名称: %s\n", courses[i].name);
                        printf("学分: %d\n", courses[i].credits);
                        printf("院系: %s\n", courses[i].department);
                        printf("学期: %s\n", courses[i].semester == SPRING ? "春季" : 
                               courses[i].semester == SUMMER ? "夏季" : 
                               courses[i].semester == FALL ? "秋季" : "冬季");
                        printf("年份: %d\n", courses[i].year);
                        printf("最大学生数: %d\n", courses[i].max_students);
                        printf("当前学生数: %d\n", courses[i].current_students);
                        printf("------------------------\n");
                        found = true;
                    }
                }
                
                if (!found) {
                    printf("您目前没有教授任何课程。\n");
                }
                break;
            }
            case 4: {
                // 查看课程学生
                printf("\n=== 查看课程学生 ===\n");
                printf("请输入课程ID: ");
                char course_id[MAX_ID_LEN];
                get_string_input("", course_id, MAX_ID_LEN);
                
                bool course_found = false;
                bool is_teacher = false;
                
                // 检查课程是否存在且属于该教师
                for (int i = 0; i < course_count; i++) {
                    if (strcmp(courses[i].course_id, course_id) == 0) {
                        course_found = true;
                        if (strcmp(courses[i].teacher_id, current_session.username) == 0) {
                            is_teacher = true;
                            break;
                        }
                    }
                }
                
                if (!course_found) {
                    printf("课程不存在。\n");
                } else if (!is_teacher) {
                    printf("您没有权限查看此课程的学生。\n");
                } else {
                    printf("\n=== 课程 %s 的学生列表 ===\n", course_id);
                    bool student_found = false;
                    
                    for (int i = 0; i < enrollment_count; i++) {
                        if (strcmp(enrollments[i].course_id, course_id) == 0) {
                            // 查找学生信息
                            for (int j = 0; j < student_count; j++) {
                                if (strcmp(students[j].id, enrollments[i].student_id) == 0) {
                                    printf("学生ID: %s\n", students[j].id);
                                    printf("姓名: %s\n", students[j].name);
                                    printf("院系: %s\n", students[j].department);
                                    printf("年级: %s\n", students[j].year == FRESHMAN ? "大一" : 
                                           students[j].year == SOPHOMORE ? "大二" : 
                                           students[j].year == JUNIOR ? "大三" : 
                                           students[j].year == SENIOR ? "大四" : "研究生");
                                    printf("平均成绩: %.2f\n", enrollments[i].average);
                                    printf("完成状态: %s\n", enrollments[i].completed ? "已完成" : "进行中");
                                    printf("------------------------\n");
                                    student_found = true;
                                    break;
                                }
                            }
                        }
                    }
                    
                    if (!student_found) {
                        printf("此课程目前没有学生。\n");
                    }
                }
                break;
            }
            case 5: {
                // 分配成绩
                assign_grade();
                break;
            }
            case 6: {
                // 生成课程报告
                printf("\n=== 生成课程报告 ===\n");
                printf("请输入课程ID: ");
                char course_id[MAX_ID_LEN];
                get_string_input("", course_id, MAX_ID_LEN);
                
                bool course_found = false;
                bool is_teacher = false;
                int course_index = -1;
                
                // 检查课程是否存在且属于该教师
                for (int i = 0; i < course_count; i++) {
                    if (strcmp(courses[i].course_id, course_id) == 0) {
                        course_found = true;
                        course_index = i;
                        if (strcmp(courses[i].teacher_id, current_session.username) == 0) {
                            is_teacher = true;
                            break;
                        }
                    }
                }
                
                if (!course_found) {
                    printf("课程不存在。\n");
                } else if (!is_teacher) {
                    printf("您没有权限生成此课程的报告。\n");
                } else {
                    printf("\n=== 课程报告: %s ===\n", courses[course_index].name);
                    printf("课程ID: %s\n", courses[course_index].course_id);
                    printf("学分: %d\n", courses[course_index].credits);
                    printf("院系: %s\n", courses[course_index].department);
                    printf("学期: %s\n", courses[course_index].semester == SPRING ? "春季" : 
                           courses[course_index].semester == SUMMER ? "夏季" : 
                           courses[course_index].semester == FALL ? "秋季" : "冬季");
                    printf("年份: %d\n", courses[course_index].year);
                    printf("最大学生数: %d\n", courses[course_index].max_students);
                    printf("当前学生数: %d\n", courses[course_index].current_students);
                    
                    // 统计学生成绩
                    int total_students = 0;
                    float total_score = 0.0;
                    int grade_counts[5] = {0}; // A, B, C, D, F
                    
                    for (int i = 0; i < enrollment_count; i++) {
                        if (strcmp(enrollments[i].course_id, course_id) == 0) {
                            total_students++;
                            total_score += enrollments[i].average;
                            
                            // 统计等级
                            if (enrollments[i].average >= 90) grade_counts[0]++;
                            else if (enrollments[i].average >= 80) grade_counts[1]++;
                            else if (enrollments[i].average >= 70) grade_counts[2]++;
                            else if (enrollments[i].average >= 60) grade_counts[3]++;
                            else grade_counts[4]++;
                        }
                    }
                    
                    if (total_students > 0) {
                        printf("\n=== 成绩统计 ===\n");
                        printf("学生总数: %d\n", total_students);
                        printf("平均分: %.2f\n", total_score / total_students);
                        printf("等级分布:\n");
                        printf("  A (90-100): %d (%.1f%%)\n", grade_counts[0], 
                               (float)grade_counts[0] / total_students * 100);
                        printf("  B (80-89): %d (%.1f%%)\n", grade_counts[1], 
                               (float)grade_counts[1] / total_students * 100);
                        printf("  C (70-79): %d (%.1f%%)\n", grade_counts[2], 
                               (float)grade_counts[2] / total_students * 100);
                        printf("  D (60-69): %d (%.1f%%)\n", grade_counts[3], 
                               (float)grade_counts[3] / total_students * 100);
                        printf("  F (0-59): %d (%.1f%%)\n", grade_counts[4], 
                               (float)grade_counts[4] / total_students * 100);
                    } else {
                        printf("\n此课程目前没有学生成绩数据。\n");
                    }
                }
                break;
            }
            case 7:
                change_password();
                break;
            case 8:
                logout();
                return;
            default:
                printf("无效选择，请重新输入。\n");
        }
    }
}

// 学生菜单
void student_menu() {
    int choice;
    
    while (1) {
        // 检查会话是否超时
        if (time(NULL) > current_session.session_timeout) {
            printf("会话已超时，请重新登录。\n");
            logout();
            return;
        }
        
        printf("\n=== 学生菜单 ===\n");
        printf("1. 查看个人信息\n");
        printf("2. 修改个人信息\n");
        printf("3. 查看已选课程\n");
        printf("4. 选课\n");
        printf("5. 退课\n");
        printf("6. 查看成绩\n");
        printf("7. 查看GPA\n");
        printf("8. 修改密码\n");
        printf("9. 登出\n");
        printf("请选择操作: ");
        
        choice = get_int_input("");
        
        switch (choice) {
            case 1: {
                // 查看学生个人信息
                Student *student = NULL;
                for (int i = 0; i < student_count; i++) {
                    if (strcmp(students[i].id, current_session.username) == 0) {
                        student = &students[i];
                        break;
                    }
                }
                
                if (student) {
                    printf("\n=== 学生信息 ===\n");
                    printf("学号: %s\n", student->id);
                    printf("姓名: %s\n", student->name);
                    printf("性别: %s\n", student->gender == MALE ? "男" : 
                           student->gender == FEMALE ? "女" : "其他");
                    printf("出生日期: %d-%d-%d\n", student->birth_date.year, 
                           student->birth_date.month, student->birth_date.day);
                    printf("地址: %s\n", student->address);
                    printf("电话: %s\n", student->phone);
                    printf("邮箱: %s\n", student->email);
                    printf("年级: %s\n", student->year == FRESHMAN ? "大一" : 
                           student->year == SOPHOMORE ? "大二" : 
                           student->year == JUNIOR ? "大三" : 
                           student->year == SENIOR ? "大四" : "研究生");
                    printf("院系: %s\n", student->department);
                    printf("GPA: %.2f\n", student->gpa);
                    printf("总学分: %d\n", student->total_credits);
                    printf("入学日期: %d-%d-%d\n", student->admission_date.year, 
                           student->admission_date.month, student->admission_date.day);
                    printf("状态: %s\n", student->is_active ? "在读" : "休学/毕业");
                    printf("班级: %s\n", student->class_id);
                } else {
                    printf("未找到学生信息。\n");
                }
                break;
            }
            case 2: {
                // 修改学生个人信息
                Student *student = NULL;
                for (int i = 0; i < student_count; i++) {
                    if (strcmp(students[i].id, current_session.username) == 0) {
                        student = &students[i];
                        break;
                    }
                }
                
                if (student) {
                    printf("\n=== 修改学生信息 ===\n");
                    printf("当前姓名: %s\n", student->name);
                    printf("新姓名 (留空保持不变): ");
                    char new_name[MAX_NAME_LEN];
                    get_string_input("", new_name, MAX_NAME_LEN);
                    if (strlen(new_name) > 0) {
                        strcpy(student->name, new_name);
                    }
                    
                    printf("当前地址: %s\n", student->address);
                    printf("新地址 (留空保持不变): ");
                    char new_address[MAX_ADDRESS_LEN];
                    get_string_input("", new_address, MAX_ADDRESS_LEN);
                    if (strlen(new_address) > 0) {
                        strcpy(student->address, new_address);
                    }
                    
                    printf("当前电话: %s\n", student->phone);
                    printf("新电话 (留空保持不变): ");
                    char new_phone[MAX_PHONE_LEN];
                    get_string_input("", new_phone, MAX_PHONE_LEN);
                    if (strlen(new_phone) > 0) {
                        if (is_valid_phone(new_phone)) {
                            strcpy(student->phone, new_phone);
                        } else {
                            printf("无效的电话号码。\n");
                        }
                    }
                    
                    printf("当前邮箱: %s\n", student->email);
                    printf("新邮箱 (留空保持不变): ");
                    char new_email[MAX_EMAIL_LEN];
                    get_string_input("", new_email, MAX_EMAIL_LEN);
                    if (strlen(new_email) > 0) {
                        if (is_valid_email(new_email)) {
                            strcpy(student->email, new_email);
                        } else {
                            printf("无效的邮箱地址。\n");
                        }
                    }
                    
                    printf("信息已更新。\n");
                } else {
                    printf("未找到学生信息。\n");
                }
                break;
            }
            case 3: {
                // 查看已选课程
                printf("\n=== 已选课程 ===\n");
                bool found = false;
                
                for (int i = 0; i < enrollment_count; i++) {
                    if (strcmp(enrollments[i].student_id, current_session.username) == 0) {
                        // 查找课程信息
                        for (int j = 0; j < course_count; j++) {
                            if (strcmp(courses[j].course_id, enrollments[i].course_id) == 0) {
                                printf("课程ID: %s\n", courses[j].course_id);
                                printf("课程名称: %s\n", courses[j].name);
                                printf("学分: %d\n", courses[j].credits);
                                printf("院系: %s\n", courses[j].department);
                                printf("学期: %s\n", courses[j].semester == SPRING ? "春季" : 
                                       courses[j].semester == SUMMER ? "夏季" : 
                                       courses[j].semester == FALL ? "秋季" : "冬季");
                                printf("年份: %d\n", courses[j].year);
                                printf("平均成绩: %.2f\n", enrollments[i].average);
                                printf("完成状态: %s\n", enrollments[i].completed ? "已完成" : "进行中");
                                printf("------------------------\n");
                                found = true;
                                break;
                            }
                        }
                    }
                }
                
                if (!found) {
                    printf("您目前没有选修任何课程。\n");
                }
                break;
            }
            case 4: {
                // 选课
                printf("\n=== 可选课程 ===\n");
                bool available = false;
                
                // 显示可选课程
                for (int i = 0; i < course_count; i++) {
                    // 检查课程是否已满
                    if (courses[i].current_students >= courses[i].max_students) {
                        continue;
                    }
                    
                    // 检查学生是否已经选了这门课
                    bool already_enrolled = false;
                    for (int j = 0; j < enrollment_count; j++) {
                        if (strcmp(enrollments[j].student_id, current_session.username) == 0 && 
                            strcmp(enrollments[j].course_id, courses[i].course_id) == 0) {
                            already_enrolled = true;
                            break;
                        }
                    }
                    
                    if (!already_enrolled) {
                        printf("课程ID: %s\n", courses[i].course_id);
                        printf("课程名称: %s\n", courses[i].name);
                        printf("学分: %d\n", courses[i].credits);
                        printf("院系: %s\n", courses[i].department);
                        printf("教师ID: %s\n", courses[i].teacher_id);
                        printf("学期: %s\n", courses[i].semester == SPRING ? "春季" : 
                               courses[i].semester == SUMMER ? "夏季" : 
                               courses[i].semester == FALL ? "秋季" : "冬季");
                        printf("年份: %d\n", courses[i].year);
                        printf("当前学生数/最大学生数: %d/%d\n", 
                               courses[i].current_students, courses[i].max_students);
                        printf("------------------------\n");
                        available = true;
                    }
                }
                
                if (!available) {
                    printf("目前没有可选课程。\n");
                } else {
                    printf("请输入要选修的课程ID: ");
                    char course_id[MAX_ID_LEN];
                    get_string_input("", course_id, MAX_ID_LEN);
                    
                    bool course_found = false;
                    bool can_enroll = false;
                    int course_index = -1;
                    
                    // 检查课程是否存在且可选
                    for (int i = 0; i < course_count; i++) {
                        if (strcmp(courses[i].course_id, course_id) == 0) {
                            course_found = true;
                            course_index = i;
                            
                            // 检查课程是否已满
                            if (courses[i].current_students >= courses[i].max_students) {
                                printf("该课程已满员。\n");
                            } else {
                                // 检查学生是否已经选了这门课
                                bool already_enrolled = false;
                                for (int j = 0; j < enrollment_count; j++) {
                                    if (strcmp(enrollments[j].student_id, current_session.username) == 0 && 
                                        strcmp(enrollments[j].course_id, course_id) == 0) {
                                        already_enrolled = true;
                                        break;
                                    }
                                }
                                
                                if (!already_enrolled) {
                                    can_enroll = true;
                                } else {
                                    printf("您已经选修了这门课程。\n");
                                }
                            }
                            break;
                        }
                    }
                    
                    if (!course_found) {
                        printf("课程不存在。\n");
                    } else if (can_enroll) {
                        // 添加选课记录
                        if (enrollment_count < MAX_ENROLLMENTS) {
                            strcpy(enrollments[enrollment_count].student_id, current_session.username);
                            strcpy(enrollments[enrollment_count].course_id, course_id);
                            
                            // 初始化成绩
                            for (int j = 0; j < MAX_GRADES; j++) {
                                enrollments[enrollment_count].grades[j] = F;
                            }
                            enrollments[enrollment_count].average = 0.0;
                            
                            // 设置选课日期为当前日期
                            time_t t = time(NULL);
                            struct tm *tm = localtime(&t);
                            enrollments[enrollment_count].enrollment_date.day = tm->tm_mday;
                            enrollments[enrollment_count].enrollment_date.month = tm->tm_mon + 1;
                            enrollments[enrollment_count].enrollment_date.year = tm->tm_year + 1900;
                            
                            enrollments[enrollment_count].completed = false;
                            
                            enrollment_count++;
                            
                            // 更新课程学生数
                            courses[course_index].current_students++;
                            
                            printf("选课成功！\n");
                        } else {
                            printf("选课记录已满，无法添加。\n");
                        }
                    }
                }
                break;
            }
            case 5: {
                // 退课
                printf("\n=== 退课 ===\n");
                printf("请输入要退选的课程ID: ");
                char course_id[MAX_ID_LEN];
                get_string_input("", course_id, MAX_ID_LEN);
                
                bool found = false;
                int enrollment_index = -1;
                int course_index = -1;
                
                // 查找选课记录
                for (int i = 0; i < enrollment_count; i++) {
                    if (strcmp(enrollments[i].student_id, current_session.username) == 0 && 
                        strcmp(enrollments[i].course_id, course_id) == 0) {
                        found = true;
                        enrollment_index = i;
                        break;
                    }
                }
                
                // 查找课程
                for (int i = 0; i < course_count; i++) {
                    if (strcmp(courses[i].course_id, course_id) == 0) {
                        course_index = i;
                        break;
                    }
                }
                
                if (!found) {
                    printf("您没有选修这门课程。\n");
                } else if (course_index == -1) {
                    printf("课程不存在。\n");
                } else {
                    // 确认退课
                    printf("您确定要退选课程 %s 吗？(y/n): ", course_id);
                    char confirm;
                    scanf(" %c", &confirm);
                    clear_input_buffer();
                    
                    if (confirm == 'y' || confirm == 'Y') {
                        // 删除选课记录
                        for (int i = enrollment_index; i < enrollment_count - 1; i++) {
                            enrollments[i] = enrollments[i + 1];
                        }
                        enrollment_count--;
                        
                        // 更新课程学生数
                        courses[course_index].current_students--;
                        
                        printf("退课成功！\n");
                    } else {
                        printf("退课已取消。\n");
                    }
                }
                break;
            }
            case 6: {
                // 查看成绩
                printf("\n=== 成绩查询 ===\n");
                bool found = false;
                
                for (int i = 0; i < enrollment_count; i++) {
                    if (strcmp(enrollments[i].student_id, current_session.username) == 0) {
                        // 查找课程信息
                        for (int j = 0; j < course_count; j++) {
                            if (strcmp(courses[j].course_id, enrollments[i].course_id) == 0) {
                                printf("课程ID: %s\n", courses[j].course_id);
                                printf("课程名称: %s\n", courses[j].name);
                                printf("学分: %d\n", courses[j].credits);
                                printf("学期: %s\n", courses[j].semester == SPRING ? "春季" : 
                                       courses[j].semester == SUMMER ? "夏季" : 
                                       courses[j].semester == FALL ? "秋季" : "冬季");
                                printf("年份: %d\n", courses[j].year);
                                printf("平均成绩: %.2f\n", enrollments[i].average);
                                printf("完成状态: %s\n", enrollments[i].completed ? "已完成" : "进行中");
                                
                                // 显示各次成绩
                                printf("各次成绩: ");
                                for (int k = 0; k < MAX_GRADES; k++) {
                                    if (enrollments[i].grades[k] != F || enrollments[i].average > 0) {
                                        printf("%d: %c ", k+1, 
                                               enrollments[i].grades[k] == A ? 'A' : 
                                               enrollments[i].grades[k] == B ? 'B' : 
                                               enrollments[i].grades[k] == C ? 'C' : 
                                               enrollments[i].grades[k] == D ? 'D' : 'F');
                                    }
                                }
                                printf("\n");
                                printf("------------------------\n");
                                found = true;
                                break;
                            }
                        }
                    }
                }
                
                if (!found) {
                    printf("您目前没有成绩记录。\n");
                }
                break;
            }
            case 7: {
                // 查看GPA
                Student *student = NULL;
                for (int i = 0; i < student_count; i++) {
                    if (strcmp(students[i].id, current_session.username) == 0) {
                        student = &students[i];
                        break;
                    }
                }
                
                if (student) {
                    printf("\n=== GPA信息 ===\n");
                    printf("当前GPA: %.2f\n", student->gpa);
                    printf("总学分: %d\n", student->total_credits);
                    
                    // 计算详细GPA信息
                    int total_courses = 0;
                    float total_points = 0.0;
                    int total_credits_earned = 0;
                    
                    for (int i = 0; i < enrollment_count; i++) {
                        if (strcmp(enrollments[i].student_id, current_session.username) == 0 && 
                            enrollments[i].completed) {
                            // 查找课程信息
                            for (int j = 0; j < course_count; j++) {
                                if (strcmp(courses[j].course_id, enrollments[i].course_id) == 0) {
                                    total_courses++;
                                    total_credits_earned += courses[j].credits;
                                    
                                    // 计算绩点
                                    float grade_point = 0.0;
                                    if (enrollments[i].average >= 90) grade_point = 4.0;
                                    else if (enrollments[i].average >= 80) grade_point = 3.0;
                                    else if (enrollments[i].average >= 70) grade_point = 2.0;
                                    else if (enrollments[i].average >= 60) grade_point = 1.0;
                                    else grade_point = 0.0;
                                    
                                    total_points += grade_point * courses[j].credits;
                                    break;
                                }
                            }
                        }
                    }
                    
                    if (total_courses > 0) {
                        printf("已完成课程数: %d\n", total_courses);
                        printf("已获得学分: %d\n", total_credits_earned);
                        printf("平均绩点: %.2f\n", total_points / total_credits_earned);
                    } else {
                        printf("您还没有完成任何课程。\n");
                    }
                } else {
                    printf("未找到学生信息。\n");
                }
                break;
            }
            case 8:
                change_password();
                break;
            case 9:
                logout();
                return;
            default:
                printf("无效选择，请重新输入。\n");
        }
    }
}

// 访客菜单
void guest_menu() {
    int choice;
    
    while (1) {
        printf("\n=== 访客菜单 ===\n");
        printf("1. 查看课程列表\n");
        printf("2. 搜索课程\n");
        printf("3. 查看教师列表\n");
        printf("4. 搜索教师\n");
        printf("5. 查看院系信息\n");
        printf("6. 返回主菜单\n");
        printf("请选择操作: ");
        
        choice = get_int_input("");
        
        switch (choice) {
            case 1:
                list_all_courses();
                break;
            case 2:
                search_course();
                break;
            case 3:
                list_all_teachers();
                break;
            case 4:
                search_teacher();
                break;
            case 5: {
                printf("\n=== 院系信息 ===\n");
                // 统计各院系信息
                char departments[MAX_DEPARTMENT_LEN][MAX_DEPARTMENT_LEN];
                int dept_count = 0;
                
                // 收集所有院系
                for (int i = 0; i < course_count; i++) {
                    bool found = false;
                    for (int j = 0; j < dept_count; j++) {
                        if (strcmp(courses[i].department, departments[j]) == 0) {
                            found = true;
                            break;
                        }
                    }
                    
                    if (!found) {
                        strcpy(departments[dept_count], courses[i].department);
                        dept_count++;
                    }
                }
                
                for (int i = 0; i < teacher_count; i++) {
                    bool found = false;
                    for (int j = 0; j < dept_count; j++) {
                        if (strcmp(teachers[i].department, departments[j]) == 0) {
                            found = true;
                            break;
                        }
                    }
                    
                    if (!found) {
                        strcpy(departments[dept_count], teachers[i].department);
                        dept_count++;
                    }
                }
                
                for (int i = 0; i < student_count; i++) {
                    bool found = false;
                    for (int j = 0; j < dept_count; j++) {
                        if (strcmp(students[i].department, departments[j]) == 0) {
                            found = true;
                            break;
                        }
                    }
                    
                    if (!found) {
                        strcpy(departments[dept_count], students[i].department);
                        dept_count++;
                    }
                }
                
                // 显示各院系统计信息
                for (int i = 0; i < dept_count; i++) {
                    printf("\n=== %s ===\n", departments[i]);
                    
                    // 统计课程数
                    int course_num = 0;
                    for (int j = 0; j < course_count; j++) {
                        if (strcmp(courses[j].department, departments[i]) == 0) {
                            course_num++;
                        }
                    }
                    printf("课程数: %d\n", course_num);
                    
                    // 统计教师数
                    int teacher_num = 0;
                    for (int j = 0; j < teacher_count; j++) {
                        if (strcmp(teachers[j].department, departments[i]) == 0 && teachers[j].is_active) {
                            teacher_num++;
                        }
                    }
                    printf("教师数: %d\n", teacher_num);
                    
                    // 统计学生数
                    int student_num = 0;
                    for (int j = 0; j < student_count; j++) {
                        if (strcmp(students[j].department, departments[i]) == 0 && students[j].is_active) {
                            student_num++;
                        }
                    }
                    printf("学生数: %d\n", student_num);
                }
                break;
            }
            case 6:
                return;
            default:
                printf("无效选择，请重新输入。\n");
        }
    }
}

// 添加学生
void add_student() {
    if (student_count >= MAX_STUDENTS) {
        printf("学生数量已达上限，无法添加。\n");
        return;
    }
    
    printf("\n=== 添加学生 ===\n");
    
    Student new_student;
    
    // 输入学号
    while (1) {
        printf("学号: ");
        get_string_input("", new_student.id, MAX_ID_LEN);
        
        // 检查学号是否已存在
        bool exists = false;
        for (int i = 0; i < student_count; i++) {
            if (strcmp(students[i].id, new_student.id) == 0) {
                exists = true;
                break;
            }
        }
        
        if (exists) {
            printf("该学号已存在，请重新输入。\n");
        } else if (!is_valid_id(new_student.id)) {
            printf("无效的学号格式，请重新输入。\n");
        } else {
            break;
        }
    }
    
    // 输入姓名
    printf("姓名: ");
    get_string_input("", new_student.name, MAX_NAME_LEN);
    
    // 输入密码
    printf("密码: ");
    get_string_input("", new_student.password, MAX_PASSWORD_LEN);
    
    // 输入性别
    new_student.gender = get_gender_input("性别 (0-男, 1-女, 2-其他): ");
    
    // 输入出生日期
    new_student.birth_date = get_date_input("出生日期 (日 月 年): ");
    
    // 输入地址
    printf("地址: ");
    get_string_input("", new_student.address, MAX_ADDRESS_LEN);
    
    // 输入电话
    while (1) {
        printf("电话: ");
        get_string_input("", new_student.phone, MAX_PHONE_LEN);
        
        if (is_valid_phone(new_student.phone)) {
            break;
        } else {
            printf("无效的电话号码，请重新输入。\n");
        }
    }
    
    // 输入邮箱
    while (1) {
        printf("邮箱: ");
        get_string_input("", new_student.email, MAX_EMAIL_LEN);
        
        if (is_valid_email(new_student.email)) {
            break;
        } else {
            printf("无效的邮箱地址，请重新输入。\n");
        }
    }
    
    // 输入年级
    new_student.year = get_year_input("年级 (0-大一, 1-大二, 2-大三, 3-大四, 4-研究生): ");
    
    // 输入院系
    printf("院系: ");
    get_string_input("", new_student.department, MAX_DEPARTMENT_LEN);
    
    // 初始化GPA和总学分
    new_student.gpa = 0.0;
    new_student.total_credits = 0;
    
    // 输入入学日期
    new_student.admission_date = get_date_input("入学日期 (日 月 年): ");
    
    // 设置状态为在读
    new_student.is_active = true;
    
    // 输入班级ID
    printf("班级ID (留空则不分配): ");
    get_string_input("", new_student.class_id, MAX_ID_LEN);
    
    // 添加到数组
    students[student_count] = new_student;
    student_count++;
    
    // 添加到哈希表
    insert_hash(&student_hash, new_student.id, &students[student_count - 1]);
    
    printf("学生添加成功！\n");
}

// 编辑学生
void edit_student() {
    printf("\n=== 编辑学生 ===\n");
    printf("请输入要编辑的学生学号: ");
    char id[MAX_ID_LEN];
    get_string_input("", id, MAX_ID_LEN);
    
    int index = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("未找到该学生。\n");
        return;
    }
    
    printf("\n当前学生信息:\n");
    printf("1. 学号: %s\n", students[index].id);
    printf("2. 姓名: %s\n", students[index].name);
    printf("3. 密码: %s\n", students[index].password);
    printf("4. 性别: %s\n", students[index].gender == MALE ? "男" : 
           students[index].gender == FEMALE ? "女" : "其他");
    printf("5. 出生日期: %d-%d-%d\n", students[index].birth_date.year, 
           students[index].birth_date.month, students[index].birth_date.day);
    printf("6. 地址: %s\n", students[index].address);
    printf("7. 电话: %s\n", students[index].phone);
    printf("8. 邮箱: %s\n", students[index].email);
    printf("9. 年级: %s\n", students[index].year == FRESHMAN ? "大一" : 
           students[index].year == SOPHOMORE ? "大二" : 
           students[index].year == JUNIOR ? "大三" : 
           students[index].year == SENIOR ? "大四" : "研究生");
    printf("10. 院系: %s\n", students[index].department);
    printf("11. GPA: %.2f\n", students[index].gpa);
    printf("12. 总学分: %d\n", students[index].total_credits);
    printf("13. 入学日期: %d-%d-%d\n", students[index].admission_date.year, 
           students[index].admission_date.month, students[index].admission_date.day);
    printf("14. 状态: %s\n", students[index].is_active ? "在读" : "休学/毕业");
    printf("15. 班级ID: %s\n", students[index].class_id);
    
    printf("\n请输入要修改的字段编号 (0-取消): ");
    int field = get_int_input("");
    
    if (field == 0) {
        printf("修改已取消。\n");
        return;
    }
    
    switch (field) {
        case 1:
            printf("学号不可修改。\n");
            break;
        case 2:
            printf("新姓名: ");
            get_string_input("", students[index].name, MAX_NAME_LEN);
            break;
        case 3:
            printf("新密码: ");
            get_string_input("", students[index].password, MAX_PASSWORD_LEN);
            break;
        case 4:
            students[index].gender = get_gender_input("新性别 (0-男, 1-女, 2-其他): ");
            break;
        case 5:
            students[index].birth_date = get_date_input("新出生日期 (日 月 年): ");
            break;
        case 6:
            printf("新地址: ");
            get_string_input("", students[index].address, MAX_ADDRESS_LEN);
            break;
        case 7:
            while (1) {
                printf("新电话: ");
                get_string_input("", students[index].phone, MAX_PHONE_LEN);
                
                if (is_valid_phone(students[index].phone)) {
                    break;
                } else {
                    printf("无效的电话号码，请重新输入。\n");
                }
            }
            break;
        case 8:
            while (1) {
                printf("新邮箱: ");
                get_string_input("", students[index].email, MAX_EMAIL_LEN);
                
                if (is_valid_email(students[index].email)) {
                    break;
                } else {
                    printf("无效的邮箱地址，请重新输入。\n");
                }
            }
            break;
        case 9:
            students[index].year = get_year_input("新年级 (0-大一, 1-大二, 2-大三, 3-大四, 4-研究生): ");
            break;
        case 10:
            printf("新院系: ");
            get_string_input("", students[index].department, MAX_DEPARTMENT_LEN);
            break;
        case 11:
            printf("新GPA: ");
            students[index].gpa = get_float_input("");
            break;
        case 12:
            printf("新总学分: ");
            students[index].total_credits = get_int_input("");
            break;
        case 13:
            students[index].admission_date = get_date_input("新入学日期 (日 月 年): ");
            break;
        case 14:
            printf("新状态 (0-休学/毕业, 1-在读): ");
            int status = get_int_input("");
            students[index].is_active = (status == 1);
            break;
        case 15:
            printf("新班级ID: ");
            get_string_input("", students[index].class_id, MAX_ID_LEN);
            break;
        default:
            printf("无效的字段编号。\n");
            return;
    }
    
    printf("学生信息已更新。\n");
}

// 删除学生
void delete_student() {
    printf("\n=== 删除学生 ===\n");
    printf("请输入要删除的学生学号: ");
    char id[MAX_ID_LEN];
    get_string_input("", id, MAX_ID_LEN);
    
    int index = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("未找到该学生。\n");
        return;
    }
    
    // 显示学生信息
    printf("\n学生信息:\n");
    printf("学号: %s\n", students[index].id);
    printf("姓名: %s\n", students[index].name);
    printf("院系: %s\n", students[index].department);
    printf("年级: %s\n", students[index].year == FRESHMAN ? "大一" : 
           students[index].year == SOPHOMORE ? "大二" : 
           students[index].year == JUNIOR ? "大三" : 
           students[index].year == SENIOR ? "大四" : "研究生");
    
    // 确认删除
    printf("\n确定要删除该学生吗？(y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    clear_input_buffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        // 从哈希表中删除
        remove_hash(&student_hash, id);
        
        // 删除相关选课记录
        for (int i = enrollment_count - 1; i >= 0; i--) {
            if (strcmp(enrollments[i].student_id, id) == 0) {
                // 更新课程学生数
                for (int j = 0; j < course_count; j++) {
                    if (strcmp(courses[j].course_id, enrollments[i].course_id) == 0) {
                        courses[j].current_students--;
                        break;
                    }
                }
                
                // 删除选课记录
                for (int k = i; k < enrollment_count - 1; k++) {
                    enrollments[k] = enrollments[k + 1];
                }
                enrollment_count--;
            }
        }
        
        // 删除学生
        for (int i = index; i < student_count - 1; i++) {
            students[i] = students[i + 1];
        }
        student_count--;
        
        printf("学生删除成功！\n");
    } else {
        printf("删除已取消。\n");
    }
}

// 查看学生
void view_student() {
    printf("\n=== 查看学生 ===\n");
    printf("请输入学生学号: ");
    char id[MAX_ID_LEN];
    get_string_input("", id, MAX_ID_LEN);
    
    int index = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("未找到该学生。\n");
        return;
    }
    
    printf("\n=== 学生信息 ===\n");
    printf("学号: %s\n", students[index].id);
    printf("姓名: %s\n", students[index].name);
    printf("性别: %s\n", students[index].gender == MALE ? "男" : 
           students[index].gender == FEMALE ? "女" : "其他");
    printf("出生日期: %d-%d-%d\n", students[index].birth_date.year, 
           students[index].birth_date.month, students[index].birth_date.day);
    printf("地址: %s\n", students[index].address);
    printf("电话: %s\n", students[index].phone);
    printf("邮箱: %s\n", students[index].email);
    printf("年级: %s\n", students[index].year == FRESHMAN ? "大一" : 
           students[index].year == SOPHOMORE ? "大二" : 
           students[index].year == JUNIOR ? "大三" : 
           students[index].year == SENIOR ? "大四" : "研究生");
    printf("院系: %s\n", students[index].department);
    printf("GPA: %.2f\n", students[index].gpa);
    printf("总学分: %d\n", students[index].total_credits);
    printf("入学日期: %d-%d-%d\n", students[index].admission_date.year, 
           students[index].admission_date.month, students[index].admission_date.day);
    printf("状态: %s\n", students[index].is_active ? "在读" : "休学/毕业");
    printf("班级ID: %s\n", students[index].class_id);
    
    // 显示选课记录
    printf("\n=== 选课记录 ===\n");
    bool found = false;
    
    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].student_id, id) == 0) {
            // 查找课程信息
            for (int j = 0; j < course_count; j++) {
                if (strcmp(courses[j].course_id, enrollments[i].course_id) == 0) {
                    printf("课程ID: %s\n", courses[j].course_id);
                    printf("课程名称: %s\n", courses[j].name);
                    printf("学分: %d\n", courses[j].credits);
                    printf("学期: %s\n", courses[j].semester == SPRING ? "春季" : 
                           courses[j].semester == SUMMER ? "夏季" : 
                           courses[j].semester == FALL ? "秋季" : "冬季");
                    printf("年份: %d\n", courses[j].year);
                    printf("平均成绩: %.2f\n", enrollments[i].average);
                    printf("完成状态: %s\n", enrollments[i].completed ? "已完成" : "进行中");
                    printf("------------------------\n");
                    found = true;
                    break;
                }
            }
        }
    }
    
    if (!found) {
        printf("该学生没有选课记录。\n");
    }
}

// 列出所有学生
void list_all_students() {
    printf("\n=== 所有学生列表 ===\n");
    
    if (student_count == 0) {
        printf("没有学生记录。\n");
        return;
    }
    
    printf("%-15s %-20s %-10s %-10s %-15s %-10s %-10s %-10s\n", 
           "学号", "姓名", "性别", "年级", "院系", "GPA", "总学分", "状态");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < student_count; i++) {
        printf("%-15s %-20s %-10s %-10s %-15s %-10.2f %-10d %-10s\n", 
               students[i].id, 
               students[i].name, 
               students[i].gender == MALE ? "男" : 
               students[i].gender == FEMALE ? "女" : "其他",
               students[i].year == FRESHMAN ? "大一" : 
               students[i].year == SOPHOMORE ? "大二" : 
               students[i].year == JUNIOR ? "大三" : 
               students[i].year == SENIOR ? "大四" : "研究生",
               students[i].department, 
               students[i].gpa, 
               students[i].total_credits, 
               students[i].is_active ? "在读" : "休学/毕业");
    }
}

// 搜索学生
void search_student() {
    printf("\n=== 搜索学生 ===\n");
    printf("1. 按学号搜索\n");
    printf("2. 按姓名搜索\n");
    printf("3. 按院系搜索\n");
    printf("4. 按班级搜索\n");
    printf("请选择搜索方式: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1: {
            printf("请输入学号: ");
            char id[MAX_ID_LEN];
            get_string_input("", id, MAX_ID_LEN);
            
            bool found = false;
            for (int i = 0; i < student_count; i++) {
                if (strcmp(students[i].id, id) == 0) {
                    printf("\n=== 学生信息 ===\n");
                    printf("学号: %s\n", students[i].id);
                    printf("姓名: %s\n", students[i].name);
                    printf("性别: %s\n", students[i].gender == MALE ? "男" : 
                           students[i].gender == FEMALE ? "女" : "其他");
                    printf("出生日期: %d-%d-%d\n", students[i].birth_date.year, 
                           students[i].birth_date.month, students[i].birth_date.day);
                    printf("地址: %s\n", students[i].address);
                    printf("电话: %s\n", students[i].phone);
                    printf("邮箱: %s\n", students[i].email);
                    printf("年级: %s\n", students[i].year == FRESHMAN ? "大一" : 
                           students[i].year == SOPHOMORE ? "大二" : 
                           students[i].year == JUNIOR ? "大三" : 
                           students[i].year == SENIOR ? "大四" : "研究生");
                    printf("院系: %s\n", students[i].department);
                    printf("GPA: %.2f\n", students[i].gpa);
                    printf("总学分: %d\n", students[i].total_credits);
                    printf("入学日期: %d-%d-%d\n", students[i].admission_date.year, 
                           students[i].admission_date.month, students[i].admission_date.day);
                    printf("状态: %s\n", students[i].is_active ? "在读" : "休学/毕业");
                    printf("班级ID: %s\n", students[i].class_id);
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                printf("未找到学号为 %s 的学生。\n", id);
            }
            break;
        }
        case 2: {
            printf("请输入姓名: ");
            char name[MAX_NAME_LEN];
            get_string_input("", name, MAX_NAME_LEN);
            
            bool found = false;
            printf("\n搜索结果:\n");
            printf("%-15s %-20s %-10s %-10s %-15s %-10s %-10s %-10s\n", 
                   "学号", "姓名", "性别", "年级", "院系", "GPA", "总学分", "状态");
            printf("--------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < student_count; i++) {
                if (strstr(students[i].name, name) != NULL) {
                    printf("%-15s %-20s %-10s %-10s %-15s %-10.2f %-10d %-10s\n", 
                           students[i].id, 
                           students[i].name, 
                           students[i].gender == MALE ? "男" : 
                           students[i].gender == FEMALE ? "女" : "其他",
                           students[i].year == FRESHMAN ? "大一" : 
                           students[i].year == SOPHOMORE ? "大二" : 
                           students[i].year == JUNIOR ? "大三" : 
                           students[i].year == SENIOR ? "大四" : "研究生",
                           students[i].department, 
                           students[i].gpa, 
                           students[i].total_credits, 
                           students[i].is_active ? "在读" : "休学/毕业");
                    found = true;
                }
            }
            
            if (!found) {
                printf("未找到姓名包含 %s 的学生。\n", name);
            }
            break;
        }
        case 3: {
            printf("请输入院系: ");
            char department[MAX_DEPARTMENT_LEN];
            get_string_input("", department, MAX_DEPARTMENT_LEN);
            
            bool found = false;
            printf("\n搜索结果:\n");
            printf("%-15s %-20s %-10s %-10s %-15s %-10s %-10s %-10s\n", 
                   "学号", "姓名", "性别", "年级", "院系", "GPA", "总学分", "状态");
            printf("--------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < student_count; i++) {
                if (strstr(students[i].department, department) != NULL) {
                    printf("%-15s %-20s %-10s %-10s %-15s %-10.2f %-10d %-10s\n", 
                           students[i].id, 
                           students[i].name, 
                           students[i].gender == MALE ? "男" : 
                           students[i].gender == FEMALE ? "女" : "其他",
                           students[i].year == FRESHMAN ? "大一" : 
                           students[i].year == SOPHOMORE ? "大二" : 
                           students[i].year == JUNIOR ? "大三" : 
                           students[i].year == SENIOR ? "大四" : "研究生",
                           students[i].department, 
                           students[i].gpa, 
                           students[i].total_credits, 
                           students[i].is_active ? "在读" : "休学/毕业");
                    found = true;
                }
            }
            
            if (!found) {
                printf("未找到院系为 %s 的学生。\n", department);
            }
            break;
        }
        case 4: {
            printf("请输入班级ID: ");
            char class_id[MAX_ID_LEN];
            get_string_input("", class_id, MAX_ID_LEN);
            
            bool found = false;
            printf("\n搜索结果:\n");
            printf("%-15s %-20s %-10s %-10s %-15s %-10s %-10s %-10s\n", 
                   "学号", "姓名", "性别", "年级", "院系", "GPA", "总学分", "状态");
            printf("--------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < student_count; i++) {
                if (strcmp(students[i].class_id, class_id) == 0) {
                    printf("%-15s %-20s %-10s %-10s %-15s %-10.2f %-10d %-10s\n", 
                           students[i].id, 
                           students[i].name, 
                           students[i].gender == MALE ? "男" : 
                           students[i].gender == FEMALE ? "女" : "其他",
                           students[i].year == FRESHMAN ? "大一" : 
                           students[i].year == SOPHOMORE ? "大二" : 
                           students[i].year == JUNIOR ? "大三" : 
                           students[i].year == SENIOR ? "大四" : "研究生",
                           students[i].department, 
                           students[i].gpa, 
                           students[i].total_credits, 
                           students[i].is_active ? "在读" : "休学/毕业");
                    found = true;
                }
            }
            
            if (!found) {
                printf("未找到班级为 %s 的学生。\n", class_id);
            }
            break;
        }
        default:
            printf("无效选择。\n");
    }
}

// 排序学生
void sort_students() {
    printf("\n=== 排序学生 ===\n");
    printf("1. 按学号排序\n");
    printf("2. 按姓名排序\n");
    printf("3. 按GPA排序\n");
    printf("4. 按总学分排序\n");
    printf("请选择排序方式: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1:
            qsort(students, student_count, sizeof(Student), compare_students_by_id);
            printf("已按学号排序。\n");
            break;
        case 2:
            qsort(students, student_count, sizeof(Student), compare_students_by_name);
            printf("已按姓名排序。\n");
            break;
        case 3:
            qsort(students, student_count, sizeof(Student), compare_students_by_gpa);
            printf("已按GPA排序。\n");
            break;
        case 4:
            // 按总学分排序需要自定义比较函数
            for (int i = 0; i < student_count - 1; i++) {
                for (int j = 0; j < student_count - i - 1; j++) {
                    if (students[j].total_credits < students[j + 1].total_credits) {
                        Student temp = students[j];
                        students[j] = students[j + 1];
                        students[j + 1] = temp;
                    }
                }
            }
            printf("已按总学分排序。\n");
            break;
        default:
            printf("无效选择。\n");
            return;
    }
    
    // 显示排序后的结果
    list_all_students();
}

// 添加课程
void add_course() {
    if (course_count >= MAX_COURSES) {
        printf("课程数量已达上限，无法添加。\n");
        return;
    }
    
    printf("\n=== 添加课程 ===\n");
    
    Course new_course;
    
    // 输入课程ID
    while (1) {
        printf("课程ID: ");
        get_string_input("", new_course.course_id, MAX_ID_LEN);
        
        // 检查课程ID是否已存在
        bool exists = false;
        for (int i = 0; i < course_count; i++) {
            if (strcmp(courses[i].course_id, new_course.course_id) == 0) {
                exists = true;
                break;
            }
        }
        
        if (exists) {
            printf("该课程ID已存在，请重新输入。\n");
        } else if (!is_valid_id(new_course.course_id)) {
            printf("无效的课程ID格式，请重新输入。\n");
        } else {
            break;
        }
    }
    
    // 输入课程名称
    printf("课程名称: ");
    get_string_input("", new_course.name, MAX_COURSE_NAME_LEN);
    
    // 输入学分
    printf("学分: ");
    new_course.credits = get_int_input("");
    
    // 输入院系
    printf("院系: ");
    get_string_input("", new_course.department, MAX_DEPARTMENT_LEN);
    
    // 输入教师ID
    printf("教师ID: ");
    get_string_input("", new_course.teacher_id, MAX_ID_LEN);
    
    // 输入学期
    new_course.semester = get_semester_input("学期 (0-春季, 1-夏季, 2-秋季, 3-冬季): ");
    
    // 输入年份
    printf("年份: ");
    new_course.year = get_int_input("");
    
    // 输入最大学生数
    printf("最大学生数: ");
    new_course.max_students = get_int_input("");
    
    // 初始化当前学生数
    new_course.current_students = 0;
    
    // 添加到数组
    courses[course_count] = new_course;
    course_count++;
    
    // 添加到哈希表
    insert_hash(&course_hash, new_course.course_id, &courses[course_count - 1]);
    
    printf("课程添加成功！\n");
}

// 编辑课程
void edit_course() {
    printf("\n=== 编辑课程 ===\n");
    printf("请输入要编辑的课程ID: ");
    char id[MAX_ID_LEN];
    get_string_input("", id, MAX_ID_LEN);
    
    int index = -1;
    for (int i = 0; i < course_count; i++) {
        if (strcmp(courses[i].course_id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("未找到该课程。\n");
        return;
    }
    
    printf("\n当前课程信息:\n");
    printf("1. 课程ID: %s\n", courses[index].course_id);
    printf("2. 课程名称: %s\n", courses[index].name);
    printf("3. 学分: %d\n", courses[index].credits);
    printf("4. 院系: %s\n", courses[index].department);
    printf("5. 教师ID: %s\n", courses[index].teacher_id);
    printf("6. 学期: %s\n", courses[index].semester == SPRING ? "春季" : 
           courses[index].semester == SUMMER ? "夏季" : 
           courses[index].semester == FALL ? "秋季" : "冬季");
    printf("7. 年份: %d\n", courses[index].year);
    printf("8. 最大学生数: %d\n", courses[index].max_students);
    printf("9. 当前学生数: %d\n", courses[index].current_students);
    
    printf("\n请输入要修改的字段编号 (0-取消): ");
    int field = get_int_input("");
    
    if (field == 0) {
        printf("修改已取消。\n");
        return;
    }
    
    switch (field) {
        case 1:
            printf("课程ID不可修改。\n");
            break;
        case 2:
            printf("新课程名称: ");
            get_string_input("", courses[index].name, MAX_COURSE_NAME_LEN);
            break;
        case 3:
            printf("新学分: ");
            courses[index].credits = get_int_input("");
            break;
        case 4:
            printf("新院系: ");
            get_string_input("", courses[index].department, MAX_DEPARTMENT_LEN);
            break;
        case 5:
            printf("新教师ID: ");
            get_string_input("", courses[index].teacher_id, MAX_ID_LEN);
            break;
        case 6:
            courses[index].semester = get_semester_input("新学期 (0-春季, 1-夏季, 2-秋季, 3-冬季): ");
            break;
        case 7:
            printf("新年份: ");
            courses[index].year = get_int_input("");
            break;
        case 8:
            printf("新最大学生数: ");
            int new_max = get_int_input("");
            if (new_max < courses[index].current_students) {
                printf("警告: 最大学生数不能小于当前学生数 (%d)。\n", courses[index].current_students);
                printf("是否继续? (y/n): ");
                char confirm;
                scanf(" %c", &confirm);
                clear_input_buffer();
                if (confirm == 'y' || confirm == 'Y') {
                    courses[index].max_students = new_max;
                }
            } else {
                courses[index].max_students = new_max;
            }
            break;
        case 9:
            printf("当前学生数不可直接修改。\n");
            break;
        default:
            printf("无效的字段编号。\n");
            return;
    }
    
    printf("课程信息已更新。\n");
}

// 删除课程
void delete_course() {
    printf("\n=== 删除课程 ===\n");
    printf("请输入要删除的课程ID: ");
    char id[MAX_ID_LEN];
    get_string_input("", id, MAX_ID_LEN);
    
    int index = -1;
    for (int i = 0; i < course_count; i++) {
        if (strcmp(courses[i].course_id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("未找到该课程。\n");
        return;
    }
    
    // 显示课程信息
    printf("\n课程信息:\n");
    printf("课程ID: %s\n", courses[index].course_id);
    printf("课程名称: %s\n", courses[index].name);
    printf("学分: %d\n", courses[index].credits);
    printf("院系: %s\n", courses[index].department);
    printf("教师ID: %s\n", courses[index].teacher_id);
    printf("学期: %s\n", courses[index].semester == SPRING ? "春季" : 
           courses[index].semester == SUMMER ? "夏季" : 
           courses[index].semester == FALL ? "秋季" : "冬季");
    printf("年份: %d\n", courses[index].year);
    printf("当前学生数: %d\n", courses[index].current_students);
    
    // 确认删除
    printf("\n确定要删除该课程吗？(y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    clear_input_buffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        // 从哈希表中删除
        remove_hash(&course_hash, id);
        
        // 删除相关选课记录
        for (int i = enrollment_count - 1; i >= 0; i--) {
            if (strcmp(enrollments[i].course_id, id) == 0) {
                // 删除选课记录
                for (int j = i; j < enrollment_count - 1; j++) {
                    enrollments[j] = enrollments[j + 1];
                }
                enrollment_count--;
            }
        }
        
        // 删除课程
        for (int i = index; i < course_count - 1; i++) {
            courses[i] = courses[i + 1];
        }
        course_count--;
        
        printf("课程删除成功！\n");
    } else {
        printf("删除已取消。\n");
    }
}

// 查看课程
void view_course() {
    printf("\n=== 查看课程 ===\n");
    printf("请输入课程ID: ");
    char id[MAX_ID_LEN];
    get_string_input("", id, MAX_ID_LEN);
    
    int index = -1;
    for (int i = 0; i < course_count; i++) {
        if (strcmp(courses[i].course_id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("未找到该课程。\n");
        return;
    }
    
    printf("\n=== 课程信息 ===\n");
    printf("课程ID: %s\n", courses[index].course_id);
    printf("课程名称: %s\n", courses[index].name);
    printf("学分: %d\n", courses[index].credits);
    printf("院系: %s\n", courses[index].department);
    printf("教师ID: %s\n", courses[index].teacher_id);
    printf("学期: %s\n", courses[index].semester == SPRING ? "春季" : 
           courses[index].semester == SUMMER ? "夏季" : 
           courses[index].semester == FALL ? "秋季" : "冬季");
    printf("年份: %d\n", courses[index].year);
    printf("最大学生数: %d\n", courses[index].max_students);
    printf("当前学生数: %d\n", courses[index].current_students);
    
    // 显示选课学生
    printf("\n=== 选课学生 ===\n");
    bool found = false;
    
    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].course_id, id) == 0) {
            // 查找学生信息
            for (int j = 0; j < student_count; j++) {
                if (strcmp(students[j].id, enrollments[i].student_id) == 0) {
                    printf("学号: %s\n", students[j].id);
                    printf("姓名: %s\n", students[j].name);
                    printf("院系: %s\n", students[j].department);
                    printf("年级: %s\n", students[j].year == FRESHMAN ? "大一" : 
                           students[j].year == SOPHOMORE ? "大二" : 
                           students[j].year == JUNIOR ? "大三" : 
                           students[j].year == SENIOR ? "大四" : "研究生");
                    printf("平均成绩: %.2f\n", enrollments[i].average);
                    printf("完成状态: %s\n", enrollments[i].completed ? "已完成" : "进行中");
                    printf("------------------------\n");
                    found = true;
                    break;
                }
            }
        }
    }
    
    if (!found) {
        printf("该课程没有选课学生。\n");
    }
}

// 列出所有课程
void list_all_courses() {
    printf("\n=== 所有课程列表 ===\n");
    
    if (course_count == 0) {
        printf("没有课程记录。\n");
        return;
    }
    
    printf("%-15s %-30s %-10s %-15s %-15s %-10s %-10s %-15s %-15s\n", 
           "课程ID", "课程名称", "学分", "院系", "教师ID", "学期", "年份", "最大学生数", "当前学生数");
    printf("------------------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < course_count; i++) {
        printf("%-15s %-30s %-10d %-15s %-15s %-10s %-10d %-15d %-15d\n", 
               courses[i].course_id, 
               courses[i].name, 
               courses[i].credits, 
               courses[i].department, 
               courses[i].teacher_id, 
               courses[i].semester == SPRING ? "春季" : 
               courses[i].semester == SUMMER ? "夏季" : 
               courses[i].semester == FALL ? "秋季" : "冬季",
               courses[i].year, 
               courses[i].max_students, 
               courses[i].current_students);
    }
}

// 搜索课程
void search_course() {
    printf("\n=== 搜索课程 ===\n");
    printf("1. 按课程ID搜索\n");
    printf("2. 按课程名称搜索\n");
    printf("3. 按院系搜索\n");
    printf("4. 按教师ID搜索\n");
    printf("请选择搜索方式: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1: {
            printf("请输入课程ID: ");
            char id[MAX_ID_LEN];
            get_string_input("", id, MAX_ID_LEN);
            
            bool found = false;
            for (int i = 0; i < course_count; i++) {
                if (strcmp(courses[i].course_id, id) == 0) {
                    printf("\n=== 课程信息 ===\n");
                    printf("课程ID: %s\n", courses[i].course_id);
                    printf("课程名称: %s\n", courses[i].name);
                    printf("学分: %d\n", courses[i].credits);
                    printf("院系: %s\n", courses[i].department);
                    printf("教师ID: %s\n", courses[i].teacher_id);
                    printf("学期: %s\n", courses[i].semester == SPRING ? "春季" : 
                           courses[i].semester == SUMMER ? "夏季" : 
                           courses[i].semester == FALL ? "秋季" : "冬季");
                    printf("年份: %d\n", courses[i].year);
                    printf("最大学生数: %d\n", courses[i].max_students);
                    printf("当前学生数: %d\n", courses[i].current_students);
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                printf("未找到课程ID为 %s 的课程。\n", id);
            }
            break;
        }
        case 2: {
            printf("请输入课程名称: ");
            char name[MAX_COURSE_NAME_LEN];
            get_string_input("", name, MAX_COURSE_NAME_LEN);
            
            bool found = false;
            printf("\n搜索结果:\n");
            printf("%-15s %-30s %-10s %-15s %-15s %-10s %-10s %-15s %-15s\n", 
                   "课程ID", "课程名称", "学分", "院系", "教师ID", "学期", "年份", "最大学生数", "当前学生数");
            printf("------------------------------------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < course_count; i++) {
                if (strstr(courses[i].name, name) != NULL) {
                    printf("%-15s %-30s %-10d %-15s %-15s %-10s %-10d %-15d %-15d\n", 
                           courses[i].course_id, 
                           courses[i].name, 
                           courses[i].credits, 
                           courses[i].department, 
                           courses[i].teacher_id, 
                           courses[i].semester == SPRING ? "春季" : 
                           courses[i].semester == SUMMER ? "夏季" : 
                           courses[i].semester == FALL ? "秋季" : "冬季",
                           courses[i].year, 
                           courses[i].max_students, 
                           courses[i].current_students);
                    found = true;
                }
            }
            
            if (!found) {
                printf("未找到课程名称包含 %s 的课程。\n", name);
            }
            break;
        }
        case 3: {
            printf("请输入院系: ");
            char department[MAX_DEPARTMENT_LEN];
            get_string_input("", department, MAX_DEPARTMENT_LEN);
            
            bool found = false;
            printf("\n搜索结果:\n");
            printf("%-15s %-30s %-10s %-15s %-15s %-10s %-10s %-15s %-15s\n", 
                   "课程ID", "课程名称", "学分", "院系", "教师ID", "学期", "年份", "最大学生数", "当前学生数");
            printf("------------------------------------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < course_count; i++) {
                if (strstr(courses[i].department, department) != NULL) {
                    printf("%-15s %-30s %-10d %-15s %-15s %-10s %-10d %-15d %-15d\n", 
                           courses[i].course_id, 
                           courses[i].name, 
                           courses[i].credits, 
                           courses[i].department, 
                           courses[i].teacher_id, 
                           courses[i].semester == SPRING ? "春季" : 
                           courses[i].semester == SUMMER ? "夏季" : 
                           courses[i].semester == FALL ? "秋季" : "冬季",
                           courses[i].year, 
                           courses[i].max_students, 
                           courses[i].current_students);
                    found = true;
                }
            }
            
            if (!found) {
                printf("未找到院系为 %s 的课程。\n", department);
            }
            break;
        }
        case 4: {
            printf("请输入教师ID: ");
            char teacher_id[MAX_ID_LEN];
            get_string_input("", teacher_id, MAX_ID_LEN);
            
            bool found = false;
            printf("\n搜索结果:\n");
            printf("%-15s %-30s %-10s %-15s %-15s %-10s %-10s %-15s %-15s\n", 
                   "课程ID", "课程名称", "学分", "院系", "教师ID", "学期", "年份", "最大学生数", "当前学生数");
            printf("------------------------------------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < course_count; i++) {
                if (strcmp(courses[i].teacher_id, teacher_id) == 0) {
                    printf("%-15s %-30s %-10d %-15s %-15s %-10s %-10d %-15d %-15d\n", 
                           courses[i].course_id, 
                           courses[i].name, 
                           courses[i].credits, 
                           courses[i].department, 
                           courses[i].teacher_id, 
                           courses[i].semester == SPRING ? "春季" : 
                           courses[i].semester == SUMMER ? "夏季" : 
                           courses[i].semester == FALL ? "秋季" : "冬季",
                           courses[i].year, 
                           courses[i].max_students, 
                           courses[i].current_students);
                    found = true;
                }
            }
            
            if (!found) {
                printf("未找到教师ID为 %s 的课程。\n", teacher_id);
            }
            break;
        }
        default:
            printf("无效选择。\n");
    }
}

// 排序课程
void sort_courses() {
    printf("\n=== 排序课程 ===\n");
    printf("1. 按课程ID排序\n");
    printf("2. 按课程名称排序\n");
    printf("3. 按学分排序\n");
    printf("4. 按当前学生数排序\n");
    printf("请选择排序方式: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1:
            qsort(courses, course_count, sizeof(Course), compare_courses_by_id);
            printf("已按课程ID排序。\n");
            break;
        case 2:
            qsort(courses, course_count, sizeof(Course), compare_courses_by_name);
            printf("已按课程名称排序。\n");
            break;
        case 3:
            qsort(courses, course_count, sizeof(Course), compare_courses_by_credits);
            printf("已按学分排序。\n");
            break;
        case 4:
            // 按当前学生数排序需要自定义比较函数
            for (int i = 0; i < course_count - 1; i++) {
                for (int j = 0; j < course_count - i - 1; j++) {
                    if (courses[j].current_students < courses[j + 1].current_students) {
                        Course temp = courses[j];
                        courses[j] = courses[j + 1];
                        courses[j + 1] = temp;
                    }
                }
            }
            printf("已按当前学生数排序。\n");
            break;
        default:
            printf("无效选择。\n");
            return;
    }
    
    // 显示排序后的结果
    list_all_courses();
}

// 学生选课
void enroll_student() {
    printf("\n=== 学生选课 ===\n");
    printf("请输入学生学号: ");
    char student_id[MAX_ID_LEN];
    get_string_input("", student_id, MAX_ID_LEN);
    
    // 检查学生是否存在
    bool student_exists = false;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, student_id) == 0 && students[i].is_active) {
            student_exists = true;
            break;
        }
    }
    
    if (!student_exists) {
        printf("学生不存在或已休学/毕业。\n");
        return;
    }
    
    printf("请输入课程ID: ");
    char course_id[MAX_ID_LEN];
    get_string_input("", course_id, MAX_ID_LEN);
    
    // 检查课程是否存在
    int course_index = -1;
    for (int i = 0; i < course_count; i++) {
        if (strcmp(courses[i].course_id, course_id) == 0) {
            course_index = i;
            break;
        }
    }
    
    if (course_index == -1) {
        printf("课程不存在。\n");
        return;
    }
    
    // 检查课程是否已满
    if (courses[course_index].current_students >= courses[course_index].max_students) {
        printf("该课程已满员。\n");
        return;
    }
    
    // 检查学生是否已经选了这门课
    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].student_id, student_id) == 0 && 
            strcmp(enrollments[i].course_id, course_id) == 0) {
            printf("该学生已经选修了这门课程。\n");
            return;
        }
    }
    
    // 添加选课记录
    if (enrollment_count < MAX_ENROLLMENTS) {
        strcpy(enrollments[enrollment_count].student_id, student_id);
        strcpy(enrollments[enrollment_count].course_id, course_id);
        
        // 初始化成绩
        for (int i = 0; i < MAX_GRADES; i++) {
            enrollments[enrollment_count].grades[i] = F;
        }
        enrollments[enrollment_count].average = 0.0;
        
        // 设置选课日期为当前日期
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        enrollments[enrollment_count].enrollment_date.day = tm->tm_mday;
        enrollments[enrollment_count].enrollment_date.month = tm->tm_mon + 1;
        enrollments[enrollment_count].enrollment_date.year = tm->tm_year + 1900;
        
        enrollments[enrollment_count].completed = false;
        
        enrollment_count++;
        
        // 更新课程学生数
        courses[course_index].current_students++;
        
        printf("选课成功！\n");
    } else {
        printf("选课记录已满，无法添加。\n");
    }
}

// 学生退课
void drop_course() {
    printf("\n=== 学生退课 ===\n");
    printf("请输入学生学号: ");
    char student_id[MAX_ID_LEN];
    get_string_input("", student_id, MAX_ID_LEN);
    
    // 检查学生是否存在
    bool student_exists = false;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, student_id) == 0) {
            student_exists = true;
            break;
        }
    }
    
    if (!student_exists) {
        printf("学生不存在。\n");
        return;
    }
    
    printf("请输入课程ID: ");
    char course_id[MAX_ID_LEN];
    get_string_input("", course_id, MAX_ID_LEN);
    
    // 查找选课记录
    int enrollment_index = -1;
    int course_index = -1;
    
    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].student_id, student_id) == 0 && 
            strcmp(enrollments[i].course_id, course_id) == 0) {
            enrollment_index = i;
            break;
        }
    }
    
    for (int i = 0; i < course_count; i++) {
        if (strcmp(courses[i].course_id, course_id) == 0) {
            course_index = i;
            break;
        }
    }
    
    if (enrollment_index == -1) {
        printf("该学生没有选修这门课程。\n");
        return;
    }
    
    if (course_index == -1) {
        printf("课程不存在。\n");
        return;
    }
    
    // 确认退课
    printf("确定要为学生 %s 退选课程 %s 吗？(y/n): ", student_id, course_id);
    char confirm;
    scanf(" %c", &confirm);
    clear_input_buffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        // 删除选课记录
        for (int i = enrollment_index; i < enrollment_count - 1; i++) {
            enrollments[i] = enrollments[i + 1];
        }
        enrollment_count--;
        
        // 更新课程学生数
        courses[course_index].current_students--;
        
        printf("退课成功！\n");
    } else {
        printf("退课已取消。\n");
    }
}

// 查看选课记录
void view_enrollments() {
    printf("\n=== 选课记录 ===\n");
    printf("1. 查看所有选课记录\n");
    printf("2. 按学生查看\n");
    printf("3. 按课程查看\n");
    printf("请选择查看方式: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1:
            if (enrollment_count == 0) {
                printf("没有选课记录。\n");
                return;
            }
            
            printf("%-15s %-15s %-10s %-10s %-15s %-10s\n", 
                   "学生ID", "课程ID", "平均成绩", "完成状态", "选课日期", "各次成绩");
            printf("----------------------------------------------------------------\n");
            
            for (int i = 0; i < enrollment_count; i++) {
                printf("%-15s %-15s %-10.2f %-10s %-15d-%d-%d ", 
                       enrollments[i].student_id, 
                       enrollments[i].course_id, 
                       enrollments[i].average, 
                       enrollments[i].completed ? "已完成" : "进行中",
                       enrollments[i].enrollment_date.year,
                       enrollments[i].enrollment_date.month,
                       enrollments[i].enrollment_date.day);
                
                // 显示各次成绩
                for (int j = 0; j < MAX_GRADES; j++) {
                    if (enrollments[i].grades[j] != F || enrollments[i].average > 0) {
                        printf("%d:%c ", j+1, 
                               enrollments[i].grades[j] == A ? 'A' : 
                               enrollments[i].grades[j] == B ? 'B' : 
                               enrollments[i].grades[j] == C ? 'C' : 
                               enrollments[i].grades[j] == D ? 'D' : 'F');
                    }
                }
                printf("\n");
            }
            break;
        case 2: {
            printf("请输入学生学号: ");
            char student_id[MAX_ID_LEN];
            get_string_input("", student_id, MAX_ID_LEN);
            
            bool found = false;
            printf("\n学生 %s 的选课记录:\n", student_id);
            printf("%-15s %-15s %-10s %-10s %-15s %-10s\n", 
                   "学生ID", "课程ID", "平均成绩", "完成状态", "选课日期", "各次成绩");
            printf("----------------------------------------------------------------\n");
            
            for (int i = 0; i < enrollment_count; i++) {
                if (strcmp(enrollments[i].student_id, student_id) == 0) {
                    printf("%-15s %-15s %-10.2f %-10s %-15d-%d-%d ", 
                           enrollments[i].student_id, 
                           enrollments[i].course_id, 
                           enrollments[i].average, 
                           enrollments[i].completed ? "已完成" : "进行中",
                           enrollments[i].enrollment_date.year,
                           enrollments[i].enrollment_date.month,
                           enrollments[i].enrollment_date.day);
                    
                    // 显示各次成绩
                    for (int j = 0; j < MAX_GRADES; j++) {
                        if (enrollments[i].grades[j] != F || enrollments[i].average > 0) {
                            printf("%d:%c ", j+1, 
                                   enrollments[i].grades[j] == A ? 'A' : 
                                   enrollments[i].grades[j] == B ? 'B' : 
                                   enrollments[i].grades[j] == C ? 'C' : 
                                   enrollments[i].grades[j] == D ? 'D' : 'F');
                        }
                    }
                    printf("\n");
                    found = true;
                }
            }
            
            if (!found) {
                printf("该学生没有选课记录。\n");
            }
            break;
        }
        case 3: {
            printf("请输入课程ID: ");
            char course_id[MAX_ID_LEN];
            get_string_input("", course_id, MAX_ID_LEN);
            
            bool found = false;
            printf("\n课程 %s 的选课记录:\n", course_id);
            printf("%-15s %-15s %-10s %-10s %-15s %-10s\n", 
                   "学生ID", "课程ID", "平均成绩", "完成状态", "选课日期", "各次成绩");
            printf("----------------------------------------------------------------\n");
            
            for (int i = 0; i < enrollment_count; i++) {
                if (strcmp(enrollments[i].course_id, course_id) == 0) {
                    printf("%-15s %-15s %-10.2f %-10s %-15d-%d-%d ", 
                           enrollments[i].student_id, 
                           enrollments[i].course_id, 
                           enrollments[i].average, 
                           enrollments[i].completed ? "已完成" : "进行中",
                           enrollments[i].enrollment_date.year,
                           enrollments[i].enrollment_date.month,
                           enrollments[i].enrollment_date.day);
                    
                    // 显示各次成绩
                    for (int j = 0; j < MAX_GRADES; j++) {
                        if (enrollments[i].grades[j] != F || enrollments[i].average > 0) {
                            printf("%d:%c ", j+1, 
                                   enrollments[i].grades[j] == A ? 'A' : 
                                   enrollments[i].grades[j] == B ? 'B' : 
                                   enrollments[i].grades[j] == C ? 'C' : 
                                   enrollments[i].grades[j] == D ? 'D' : 'F');
                        }
                    }
                    printf("\n");
                    found = true;
                }
            }
            
            if (!found) {
                printf("该课程没有选课记录。\n");
            }
            break;
        }
        default:
            printf("无效选择。\n");
    }
}

// 分配成绩
void assign_grade() {
    printf("\n=== 分配成绩 ===\n");
    printf("请输入学生学号: ");
    char student_id[MAX_ID_LEN];
    get_string_input("", student_id, MAX_ID_LEN);
    
    printf("请输入课程ID: ");
    char course_id[MAX_ID_LEN];
    get_string_input("", course_id, MAX_ID_LEN);
    
    // 查找选课记录
    int enrollment_index = -1;
    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].student_id, student_id) == 0 && 
            strcmp(enrollments[i].course_id, course_id) == 0) {
            enrollment_index = i;
            break;
        }
    }
    
    if (enrollment_index == -1) {
        printf("未找到该学生的选课记录。\n");
        return;
    }
    
    // 显示当前成绩
    printf("\n当前成绩记录:\n");
    printf("平均成绩: %.2f\n", enrollments[enrollment_index].average);
    printf("各次成绩: ");
    for (int i = 0; i < MAX_GRADES; i++) {
        if (enrollments[enrollment_index].grades[i] != F || enrollments[enrollment_index].average > 0) {
            printf("%d: %c ", i+1, 
                   enrollments[enrollment_index].grades[i] == A ? 'A' : 
                   enrollments[enrollment_index].grades[i] == B ? 'B' : 
                   enrollments[enrollment_index].grades[i] == C ? 'C' : 
                   enrollments[enrollment_index].grades[i] == D ? 'D' : 'F');
        }
    }
    printf("\n");
    
    // 输入新的成绩
    printf("\n请输入新的成绩 (输入-1结束):\n");
    float total = 0.0;
    int count = 0;
    
    for (int i = 0; i < MAX_GRADES; i++) {
        printf("成绩 %d (0-100): ", i+1);
        float score = get_float_input("");
        
        if (score < 0) {
            break;
        }
        
        if (score > 100) {
            printf("成绩不能超过100，请重新输入。\n");
            i--;
            continue;
        }
        
        // 转换为等级
        if (score >= 90) enrollments[enrollment_index].grades[i] = A;
        else if (score >= 80) enrollments[enrollment_index].grades[i] = B;
        else if (score >= 70) enrollments[enrollment_index].grades[i] = C;
        else if (score >= 60) enrollments[enrollment_index].grades[i] = D;
        else enrollments[enrollment_index].grades[i] = F;
        
        total += score;
        count++;
    }
    
    if (count > 0) {
        // 计算平均成绩
        enrollments[enrollment_index].average = total / count;
        
        // 检查是否完成课程
        if (enrollments[enrollment_index].average >= 60) {
            enrollments[enrollment_index].completed = true;
            
            // 更新学生的GPA和总学分
            for (int i = 0; i < student_count; i++) {
                if (strcmp(students[i].id, student_id) == 0) {
                    // 查找课程学分
                    int credits = 0;
                    for (int j = 0; j < course_count; j++) {
                        if (strcmp(courses[j].course_id, course_id) == 0) {
                            credits = courses[j].credits;
                            break;
                        }
                    }
                    
                    // 计算绩点
                    float grade_point = 0.0;
                    if (enrollments[enrollment_index].average >= 90) grade_point = 4.0;
                    else if (enrollments[enrollment_index].average >= 80) grade_point = 3.0;
                    else if (enrollments[enrollment_index].average >= 70) grade_point = 2.0;
                    else if (enrollments[enrollment_index].average >= 60) grade_point = 1.0;
                    else grade_point = 0.0;
                    
                    // 更新GPA
                    float total_points = students[i].gpa * students[i].total_credits;
                    students[i].total_credits += credits;
                    students[i].gpa = (total_points + grade_point * credits) / students[i].total_credits;
                    
                    break;
                }
            }
        }
        
        printf("成绩分配成功！\n");
        printf("新的平均成绩: %.2f\n", enrollments[enrollment_index].average);
        printf("完成状态: %s\n", enrollments[enrollment_index].completed ? "已完成" : "进行中");
    } else {
        printf("没有输入新的成绩。\n");
    }
}

// 计算GPA
void calculate_gpa() {
    printf("\n=== 计算GPA ===\n");
    printf("请输入学生学号: ");
    char student_id[MAX_ID_LEN];
    get_string_input("", student_id, MAX_ID_LEN);
    
    // 查找学生
    int student_index = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, student_id) == 0) {
            student_index = i;
            break;
        }
    }
    
    if (student_index == -1) {
        printf("未找到该学生。\n");
        return;
    }
    
    // 计算GPA
    float total_points = 0.0;
    int total_credits = 0;
    
    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].student_id, student_id) == 0 && 
            enrollments[i].completed) {
            // 查找课程学分
            int credits = 0;
            for (int j = 0; j < course_count; j++) {
                if (strcmp(courses[j].course_id, enrollments[i].course_id) == 0) {
                    credits = courses[j].credits;
                    break;
                }
            }
            
            // 计算绩点
            float grade_point = 0.0;
            if (enrollments[i].average >= 90) grade_point = 4.0;
            else if (enrollments[i].average >= 80) grade_point = 3.0;
            else if (enrollments[i].average >= 70) grade_point = 2.0;
            else if (enrollments[i].average >= 60) grade_point = 1.0;
            else grade_point = 0.0;
            
            total_points += grade_point * credits;
            total_credits += credits;
        }
    }
    
    if (total_credits > 0) {
        students[student_index].gpa = total_points / total_credits;
        students[student_index].total_credits = total_credits;
        
        printf("\nGPA计算结果:\n");
        printf("学生: %s\n", students[student_index].name);
        printf("总绩点: %.2f\n", total_points);
        printf("总学分: %d\n", total_credits);
        printf("GPA: %.2f\n", students[student_index].gpa);
    } else {
        printf("该学生没有完成任何课程，无法计算GPA。\n");
    }
}

// 添加教师
void add_teacher() {
    if (teacher_count >= MAX_TEACHERS) {
        printf("教师数量已达上限，无法添加。\n");
        return;
    }
    
    printf("\n=== 添加教师 ===\n");
    
    Teacher new_teacher;
    
    // 输入教师ID
    while (1) {
        printf("教师ID: ");
        get_string_input("", new_teacher.id, MAX_ID_LEN);
        
        // 检查教师ID是否已存在
        bool exists = false;
        for (int i = 0; i < teacher_count; i++) {
            if (strcmp(teachers[i].id, new_teacher.id) == 0) {
                exists = true;
                break;
            }
        }
        
        if (exists) {
            printf("该教师ID已存在，请重新输入。\n");
        } else if (!is_valid_id(new_teacher.id)) {
            printf("无效的教师ID格式，请重新输入。\n");
        } else {
            break;
        }
    }
    
    // 输入姓名
    printf("姓名: ");
    get_string_input("", new_teacher.name, MAX_NAME_LEN);
    
    // 输入密码
    printf("密码: ");
    get_string_input("", new_teacher.password, MAX_PASSWORD_LEN);
    
    // 输入性别
    new_teacher.gender = get_gender_input("性别 (0-男, 1-女, 2-其他): ");
    
    // 输入出生日期
    new_teacher.birth_date = get_date_input("出生日期 (日 月 年): ");
    
    // 输入院系
    printf("院系: ");
    get_string_input("", new_teacher.department, MAX_DEPARTMENT_LEN);
    
    // 输入电话
    while (1) {
        printf("电话: ");
        get_string_input("", new_teacher.phone, MAX_PHONE_LEN);
        
        if (is_valid_phone(new_teacher.phone)) {
            break;
        } else {
            printf("无效的电话号码，请重新输入。\n");
        }
    }
    
    // 输入邮箱
    while (1) {
        printf("邮箱: ");
        get_string_input("", new_teacher.email, MAX_EMAIL_LEN);
        
        if (is_valid_email(new_teacher.email)) {
            break;
        } else {
            printf("无效的邮箱地址，请重新输入。\n");
        }
    }
    
    // 输入聘用日期
    new_teacher.hire_date = get_date_input("聘用日期 (日 月 年): ");
    
    // 输入职称
    printf("职称: ");
    get_string_input("", new_teacher.title, MAX_NAME_LEN);
    
    // 输入薪资
    printf("薪资: ");
    new_teacher.salary = get_float_input("");
    
    // 设置状态为在职
    new_teacher.is_active = true;
    
    // 添加到数组
    teachers[teacher_count] = new_teacher;
    teacher_count++;
    
    // 添加到哈希表
    insert_hash(&teacher_hash, new_teacher.id, &teachers[teacher_count - 1]);
    
    printf("教师添加成功！\n");
}

// 编辑教师
void edit_teacher() {
    printf("\n=== 编辑教师 ===\n");
    printf("请输入要编辑的教师ID: ");
    char id[MAX_ID_LEN];
    get_string_input("", id, MAX_ID_LEN);
    
    int index = -1;
    for (int i = 0; i < teacher_count; i++) {
        if (strcmp(teachers[i].id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("未找到该教师。\n");
        return;
    }
    
    printf("\n当前教师信息:\n");
    printf("1. 教师ID: %s\n", teachers[index].id);
    printf("2. 姓名: %s\n", teachers[index].name);
    printf("3. 密码: %s\n", teachers[index].password);
    printf("4. 性别: %s\n", teachers[index].gender == MALE ? "男" : 
           teachers[index].gender == FEMALE ? "女" : "其他");
    printf("5. 出生日期: %d-%d-%d\n", teachers[index].birth_date.year, 
           teachers[index].birth_date.month, teachers[index].birth_date.day);
    printf("6. 院系: %s\n", teachers[index].department);
    printf("7. 电话: %s\n", teachers[index].phone);
    printf("8. 邮箱: %s\n", teachers[index].email);
    printf("9. 聘用日期: %d-%d-%d\n", teachers[index].hire_date.year, 
           teachers[index].hire_date.month, teachers[index].hire_date.day);
    printf("10. 职称: %s\n", teachers[index].title);
    printf("11. 薪资: %.2f\n", teachers[index].salary);
    printf("12. 状态: %s\n", teachers[index].is_active ? "在职" : "离职");
    
    printf("\n请输入要修改的字段编号 (0-取消): ");
    int field = get_int_input("");
    
    if (field == 0) {
        printf("修改已取消。\n");
        return;
    }
    
    switch (field) {
        case 1:
            printf("教师ID不可修改。\n");
            break;
        case 2:
            printf("新姓名: ");
            get_string_input("", teachers[index].name, MAX_NAME_LEN);
            break;
        case 3:
            printf("新密码: ");
            get_string_input("", teachers[index].password, MAX_PASSWORD_LEN);
            break;
        case 4:
            teachers[index].gender = get_gender_input("新性别 (0-男, 1-女, 2-其他): ");
            break;
        case 5:
            teachers[index].birth_date = get_date_input("新出生日期 (日 月 年): ");
            break;
        case 6:
            printf("新院系: ");
            get_string_input("", teachers[index].department, MAX_DEPARTMENT_LEN);
            break;
        case 7:
            while (1) {
                printf("新电话: ");
                get_string_input("", teachers[index].phone, MAX_PHONE_LEN);
                
                if (is_valid_phone(teachers[index].phone)) {
                    break;
                } else {
                    printf("无效的电话号码，请重新输入。\n");
                }
            }
            break;
        case 8:
            while (1) {
                printf("新邮箱: ");
                get_string_input("", teachers[index].email, MAX_EMAIL_LEN);
                
                if (is_valid_email(teachers[index].email)) {
                    break;
                } else {
                    printf("无效的邮箱地址，请重新输入。\n");
                }
            }
            break;
        case 9:
            teachers[index].hire_date = get_date_input("新聘用日期 (日 月 年): ");
            break;
        case 10:
            printf("新职称: ");
            get_string_input("", teachers[index].title, MAX_NAME_LEN);
            break;
        case 11:
            printf("新薪资: ");
            teachers[index].salary = get_float_input("");
            break;
        case 12:
            printf("新状态 (0-离职, 1-在职): ");
            int status = get_int_input("");
            teachers[index].is_active = (status == 1);
            break;
        default:
            printf("无效的字段编号。\n");
            return;
    }
    
    printf("教师信息已更新。\n");
}

// 删除教师
void delete_teacher() {
    printf("\n=== 删除教师 ===\n");
    printf("请输入要删除的教师ID: ");
    char id[MAX_ID_LEN];
    get_string_input("", id, MAX_ID_LEN);
    
    int index = -1;
    for (int i = 0; i < teacher_count; i++) {
        if (strcmp(teachers[i].id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("未找到该教师。\n");
        return;
    }
    
    // 显示教师信息
    printf("\n教师信息:\n");
    printf("教师ID: %s\n", teachers[index].id);
    printf("姓名: %s\n", teachers[index].name);
    printf("院系: %s\n", teachers[index].department);
    printf("职称: %s\n", teachers[index].title);
    
    // 确认删除
    printf("\n确定要删除该教师吗？(y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    clear_input_buffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        // 从哈希表中删除
        remove_hash(&teacher_hash, id);
        
        // 检查是否有课程由该教师教授
        for (int i = 0; i < course_count; i++) {
            if (strcmp(courses[i].teacher_id, id) == 0) {
                printf("警告: 该教师教授的课程 %s 需要重新分配教师。\n", courses[i].course_id);
            }
        }
        
        // 检查是否有班级由该教师负责
        for (int i = 0; i < class_count; i++) {
            if (strcmp(classes[i].teacher_id, id) == 0) {
                printf("警告: 该教师负责的班级 %s 需要重新分配教师。\n", classes[i].id);
            }
        }
        
        // 删除教师
        for (int i = index; i < teacher_count - 1; i++) {
            teachers[i] = teachers[i + 1];
        }
        teacher_count--;
        
        printf("教师删除成功！\n");
    } else {
        printf("删除已取消。\n");
    }
}

// 查看教师
void view_teacher() {
    printf("\n=== 查看教师 ===\n");
    printf("请输入教师ID: ");
    char id[MAX_ID_LEN];
    get_string_input("", id, MAX_ID_LEN);
    
    int index = -1;
    for (int i = 0; i < teacher_count; i++) {
        if (strcmp(teachers[i].id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("未找到该教师。\n");
        return;
    }
    
    printf("\n=== 教师信息 ===\n");
    printf("教师ID: %s\n", teachers[index].id);
    printf("姓名: %s\n", teachers[index].name);
    printf("性别: %s\n", teachers[index].gender == MALE ? "男" : 
           teachers[index].gender == FEMALE ? "女" : "其他");
    printf("出生日期: %d-%d-%d\n", teachers[index].birth_date.year, 
           teachers[index].birth_date.month, teachers[index].birth_date.day);
    printf("院系: %s\n", teachers[index].department);
    printf("电话: %s\n", teachers[index].phone);
    printf("邮箱: %s\n", teachers[index].email);
    printf("聘用日期: %d-%d-%d\n", teachers[index].hire_date.year, 
           teachers[index].hire_date.month, teachers[index].hire_date.day);
    printf("职称: %s\n", teachers[index].title);
    printf("薪资: %.2f\n", teachers[index].salary);
    printf("状态: %s\n", teachers[index].is_active ? "在职" : "离职");
    
    // 显示教授的课程
    printf("\n=== 教授的课程 ===\n");
    bool found = false;
    
    for (int i = 0; i < course_count; i++) {
        if (strcmp(courses[i].teacher_id, id) == 0) {
            printf("课程ID: %s\n", courses[i].course_id);
            printf("课程名称: %s\n", courses[i].name);
            printf("学分: %d\n", courses[i].credits);
            printf("学期: %s\n", courses[i].semester == SPRING ? "春季" : 
                   courses[i].semester == SUMMER ? "夏季" : 
                   courses[i].semester == FALL ? "秋季" : "冬季");
            printf("年份: %d\n", courses[i].year);
            printf("当前学生数: %d\n", courses[i].current_students);
            printf("------------------------\n");
            found = true;
        }
    }
    
    if (!found) {
        printf("该教师目前没有教授任何课程。\n");
    }
    
    // 显示负责的班级
    printf("\n=== 负责的班级 ===\n");
    found = false;
    
    for (int i = 0; i < class_count; i++) {
        if (strcmp(classes[i].teacher_id, id) == 0) {
            printf("班级ID: %s\n", classes[i].id);
            printf("班级名称: %s\n", classes[i].name);
            printf("院系: %s\n", classes[i].department);
            printf("学生数: %d/%d\n", classes[i].student_count, classes[i].max_students);
            printf("------------------------\n");
            found = true;
        }
    }
    
    if (!found) {
        printf("该教师目前没有负责任何班级。\n");
    }
}

// 列出所有教师
void list_all_teachers() {
    printf("\n=== 所有教师列表 ===\n");
    
    if (teacher_count == 0) {
        printf("没有教师记录。\n");
        return;
    }
    
    printf("%-15s %-20s %-10s %-15s %-20s %-15s %-10s\n", 
           "教师ID", "姓名", "性别", "院系", "职称", "聘用日期", "状态");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < teacher_count; i++) {
        printf("%-15s %-20s %-10s %-15s %-20s %-15d-%d-%d %-10s\n", 
               teachers[i].id, 
               teachers[i].name, 
               teachers[i].gender == MALE ? "男" : 
               teachers[i].gender == FEMALE ? "女" : "其他",
               teachers[i].department, 
               teachers[i].title, 
               teachers[i].hire_date.year,
               teachers[i].hire_date.month,
               teachers[i].hire_date.day,
               teachers[i].is_active ? "在职" : "离职");
    }
}

// 搜索教师
void search_teacher() {
    printf("\n=== 搜索教师 ===\n");
    printf("1. 按教师ID搜索\n");
    printf("2. 按姓名搜索\n");
    printf("3. 按院系搜索\n");
    printf("4. 按职称搜索\n");
    printf("请选择搜索方式: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1: {
            printf("请输入教师ID: ");
            char id[MAX_ID_LEN];
            get_string_input("", id, MAX_ID_LEN);
            
            bool found = false;
            for (int i = 0; i < teacher_count; i++) {
                if (strcmp(teachers[i].id, id) == 0) {
                    printf("\n=== 教师信息 ===\n");
                    printf("教师ID: %s\n", teachers[i].id);
                    printf("姓名: %s\n", teachers[i].name);
                    printf("性别: %s\n", teachers[i].gender == MALE ? "男" : 
                           teachers[i].gender == FEMALE ? "女" : "其他");
                    printf("出生日期: %d-%d-%d\n", teachers[i].birth_date.year, 
                           teachers[i].birth_date.month, teachers[i].birth_date.day);
                    printf("院系: %s\n", teachers[i].department);
                    printf("电话: %s\n", teachers[i].phone);
                    printf("邮箱: %s\n", teachers[i].email);
                    printf("聘用日期: %d-%d-%d\n", teachers[i].hire_date.year, 
                           teachers[i].hire_date.month, teachers[i].hire_date.day);
                    printf("职称: %s\n", teachers[i].title);
                    printf("薪资: %.2f\n", teachers[i].salary);
                    printf("状态: %s\n", teachers[i].is_active ? "在职" : "离职");
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                printf("未找到教师ID为 %s 的教师。\n", id);
            }
            break;
        }
        case 2: {
            printf("请输入姓名: ");
            char name[MAX_NAME_LEN];
            get_string_input("", name, MAX_NAME_LEN);
            
            bool found = false;
            printf("\n搜索结果:\n");
            printf("%-15s %-20s %-10s %-15s %-20s %-15s %-10s\n", 
                   "教师ID", "姓名", "性别", "院系", "职称", "聘用日期", "状态");
            printf("--------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < teacher_count; i++) {
                if (strstr(teachers[i].name, name) != NULL) {
                    printf("%-15s %-20s %-10s %-15s %-20s %-15d-%d-%d %-10s\n", 
                           teachers[i].id, 
                           teachers[i].name, 
                           teachers[i].gender == MALE ? "男" : 
                           teachers[i].gender == FEMALE ? "女" : "其他",
                           teachers[i].department, 
                           teachers[i].title, 
                           teachers[i].hire_date.year,
                           teachers[i].hire_date.month,
                           teachers[i].hire_date.day,
                           teachers[i].is_active ? "在职" : "离职");
                    found = true;
                }
            }
            
            if (!found) {
                printf("未找到姓名包含 %s 的教师。\n", name);
            }
            break;
        }
        case 3: {
            printf("请输入院系: ");
            char department[MAX_DEPARTMENT_LEN];
            get_string_input("", department, MAX_DEPARTMENT_LEN);
            
            bool found = false;
            printf("\n搜索结果:\n");
            printf("%-15s %-20s %-10s %-15s %-20s %-15s %-10s\n", 
                   "教师ID", "姓名", "性别", "院系", "职称", "聘用日期", "状态");
            printf("--------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < teacher_count; i++) {
                if (strstr(teachers[i].department, department) != NULL) {
                    printf("%-15s %-20s %-10s %-15s %-20s %-15d-%d-%d %-10s\n", 
                           teachers[i].id, 
                           teachers[i].name, 
                           teachers[i].gender == MALE ? "男" : 
                           teachers[i].gender == FEMALE ? "女" : "其他",
                           teachers[i].department, 
                           teachers[i].title, 
                           teachers[i].hire_date.year,
                           teachers[i].hire_date.month,
                           teachers[i].hire_date.day,
                           teachers[i].is_active ? "在职" : "离职");
                    found = true;
                }
            }
            
            if (!found) {
                printf("未找到院系为 %s 的教师。\n", department);
            }
            break;
        }
        case 4: {
            printf("请输入职称: ");
            char title[MAX_NAME_LEN];
            get_string_input("", title, MAX_NAME_LEN);
            
            bool found = false;
            printf("\n搜索结果:\n");
            printf("%-15s %-20s %-10s %-15s %-20s %-15s %-10s\n", 
                   "教师ID", "姓名", "性别", "院系", "职称", "聘用日期", "状态");
            printf("--------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < teacher_count; i++) {
                if (strstr(teachers[i].title, title) != NULL) {
                    printf("%-15s %-20s %-10s %-15s %-20s %-15d-%d-%d %-10s\n", 
                           teachers[i].id, 
                           teachers[i].name, 
                           teachers[i].gender == MALE ? "男" : 
                           teachers[i].gender == FEMALE ? "女" : "其他",
                           teachers[i].department, 
                           teachers[i].title, 
                           teachers[i].hire_date.year,
                           teachers[i].hire_date.month,
                           teachers[i].hire_date.day,
                           teachers[i].is_active ? "在职" : "离职");
                    found = true;
                }
            }
            
            if (!found) {
                printf("未找到职称为 %s 的教师。\n", title);
            }
            break;
        }
        default:
            printf("无效选择。\n");
    }
}

// 排序教师
void sort_teachers() {
    printf("\n=== 排序教师 ===\n");
    printf("1. 按教师ID排序\n");
    printf("2. 按姓名排序\n");
    printf("3. 按薪资排序\n");
    printf("4. 按聘用日期排序\n");
    printf("请选择排序方式: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1:
            qsort(teachers, teacher_count, sizeof(Teacher), compare_teachers_by_id);
            printf("已按教师ID排序。\n");
            break;
        case 2:
            qsort(teachers, teacher_count, sizeof(Teacher), compare_teachers_by_name);
            printf("已按姓名排序。\n");
            break;
        case 3:
            qsort(teachers, teacher_count, sizeof(Teacher), compare_teachers_by_salary);
            printf("已按薪资排序。\n");
            break;
        case 4:
            // 按聘用日期排序需要自定义比较函数
            for (int i = 0; i < teacher_count - 1; i++) {
                for (int j = 0; j < teacher_count - i - 1; j++) {
                    if (teachers[j].hire_date.year > teachers[j + 1].hire_date.year || 
                        (teachers[j].hire_date.year == teachers[j + 1].hire_date.year && 
                         teachers[j].hire_date.month > teachers[j + 1].hire_date.month) || 
                        (teachers[j].hire_date.year == teachers[j + 1].hire_date.year && 
                         teachers[j].hire_date.month == teachers[j + 1].hire_date.month && 
                         teachers[j].hire_date.day > teachers[j + 1].hire_date.day)) {
                        Teacher temp = teachers[j];
                        teachers[j] = teachers[j + 1];
                        teachers[j + 1] = temp;
                    }
                }
            }
            printf("已按聘用日期排序。\n");
            break;
        default:
            printf("无效选择。\n");
            return;
    }
    
    // 显示排序后的结果
    list_all_teachers();
}

// 添加班级
void add_class() {
    if (class_count >= MAX_CLASSES) {
        printf("班级数量已达上限，无法添加。\n");
        return;
    }
    
    printf("\n=== 添加班级 ===\n");
    
    Class new_class;
    
    // 输入班级ID
    while (1) {
        printf("班级ID: ");
        get_string_input("", new_class.id, MAX_ID_LEN);
        
        // 检查班级ID是否已存在
        bool exists = false;
        for (int i = 0; i < class_count; i++) {
            if (strcmp(classes[i].id, new_class.id) == 0) {
                exists = true;
                break;
            }
        }
        
        if (exists) {
            printf("该班级ID已存在，请重新输入。\n");
        } else if (!is_valid_id(new_class.id)) {
            printf("无效的班级ID格式，请重新输入。\n");
        } else {
            break;
        }
    }
    
    // 输入班级名称
    printf("班级名称: ");
    get_string_input("", new_class.name, MAX_NAME_LEN);
    
    // 输入院系
    printf("院系: ");
    get_string_input("", new_class.department, MAX_DEPARTMENT_LEN);
    
    // 输入教师ID
    printf("教师ID (留空则不分配): ");
    get_string_input("", new_class.teacher_id, MAX_ID_LEN);
    
    // 如果输入了教师ID，检查是否存在
    if (strlen(new_class.teacher_id) > 0) {
        bool teacher_exists = false;
        for (int i = 0; i < teacher_count; i++) {
            if (strcmp(teachers[i].id, new_class.teacher_id) == 0 && teachers[i].is_active) {
                teacher_exists = true;
                break;
            }
        }
        
        if (!teacher_exists) {
            printf("警告: 教师ID不存在或教师已离职。\n");
        }
    }
    
    // 输入最大学生数
    printf("最大学生数: ");
    new_class.max_students = get_int_input("");
    
    // 初始化学生数
    new_class.student_count = 0;
    
    // 添加到数组
    classes[class_count] = new_class;
    class_count++;
    
    // 添加到哈希表
    insert_hash(&class_hash, new_class.id, &classes[class_count - 1]);
    
    printf("班级添加成功！\n");
}

// 编辑班级
void edit_class() {
    printf("\n=== 编辑班级 ===\n");
    printf("请输入要编辑的班级ID: ");
    char id[MAX_ID_LEN];
    get_string_input("", id, MAX_ID_LEN);
    
    int index = -1;
    for (int i = 0; i < class_count; i++) {
        if (strcmp(classes[i].id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("未找到该班级。\n");
        return;
    }
    
    printf("\n当前班级信息:\n");
    printf("1. 班级ID: %s\n", classes[index].id);
    printf("2. 班级名称: %s\n", classes[index].name);
    printf("3. 院系: %s\n", classes[index].department);
    printf("4. 教师ID: %s\n", classes[index].teacher_id);
    printf("5. 学生数: %d\n", classes[index].student_count);
    printf("6. 最大学生数: %d\n", classes[index].max_students);
    
    printf("\n请输入要修改的字段编号 (0-取消): ");
    int field = get_int_input("");
    
    if (field == 0) {
        printf("修改已取消。\n");
        return;
    }
    
    switch (field) {
        case 1:
            printf("班级ID不可修改。\n");
            break;
        case 2:
            printf("新班级名称: ");
            get_string_input("", classes[index].name, MAX_NAME_LEN);
            break;
        case 3:
            printf("新院系: ");
            get_string_input("", classes[index].department, MAX_DEPARTMENT_LEN);
            break;
        case 4:
            printf("新教师ID: ");
            get_string_input("", classes[index].teacher_id, MAX_ID_LEN);
            
            // 如果输入了教师ID，检查是否存在
            if (strlen(classes[index].teacher_id) > 0) {
                bool teacher_exists = false;
                for (int i = 0; i < teacher_count; i++) {
                    if (strcmp(teachers[i].id, classes[index].teacher_id) == 0 && teachers[i].is_active) {
                        teacher_exists = true;
                        break;
                    }
                }
                
                if (!teacher_exists) {
                    printf("警告: 教师ID不存在或教师已离职。\n");
                }
            }
            break;
        case 5:
            printf("学生数不可直接修改。\n");
            break;
        case 6:
            printf("新最大学生数: ");
            int new_max = get_int_input("");
            if (new_max < classes[index].student_count) {
                printf("警告: 最大学生数不能小于当前学生数 (%d)。\n", classes[index].student_count);
                printf("是否继续? (y/n): ");
                char confirm;
                scanf(" %c", &confirm);
                clear_input_buffer();
                if (confirm == 'y' || confirm == 'Y') {
                    classes[index].max_students = new_max;
                }
            } else {
                classes[index].max_students = new_max;
            }
            break;
        default:
            printf("无效的字段编号。\n");
            return;
    }
    
    printf("班级信息已更新。\n");
}

// 删除班级
void delete_class() {
    printf("\n=== 删除班级 ===\n");
    printf("请输入要删除的班级ID: ");
    char id[MAX_ID_LEN];
    get_string_input("", id, MAX_ID_LEN);
    
    int index = -1;
    for (int i = 0; i < class_count; i++) {
        if (strcmp(classes[i].id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("未找到该班级。\n");
        return;
    }
    
    // 显示班级信息
    printf("\n班级信息:\n");
    printf("班级ID: %s\n", classes[index].id);
    printf("班级名称: %s\n", classes[index].name);
    printf("院系: %s\n", classes[index].department);
    printf("教师ID: %s\n", classes[index].teacher_id);
    printf("学生数: %d\n", classes[index].student_count);
    
    // 确认删除
    printf("\n确定要删除该班级吗？(y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    clear_input_buffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        // 从哈希表中删除
        remove_hash(&class_hash, id);
        
        // 更新学生的班级ID
        for (int i = 0; i < student_count; i++) {
            if (strcmp(students[i].class_id, id) == 0) {
                strcpy(students[i].class_id, "");
            }
        }
        
        // 删除班级
        for (int i = index; i < class_count - 1; i++) {
            classes[i] = classes[i + 1];
        }
        class_count--;
        
        printf("班级删除成功！\n");
    } else {
        printf("删除已取消。\n");
    }
}

// 查看班级
void view_class() {
    printf("\n=== 查看班级 ===\n");
    printf("请输入班级ID: ");
    char id[MAX_ID_LEN];
    get_string_input("", id, MAX_ID_LEN);
    
    int index = -1;
    for (int i = 0; i < class_count; i++) {
        if (strcmp(classes[i].id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("未找到该班级。\n");
        return;
    }
    
    printf("\n=== 班级信息 ===\n");
    printf("班级ID: %s\n", classes[index].id);
    printf("班级名称: %s\n", classes[index].name);
    printf("院系: %s\n", classes[index].department);
    printf("教师ID: %s\n", classes[index].teacher_id);
    printf("学生数: %d\n", classes[index].student_count);
    printf("最大学生数: %d\n", classes[index].max_students);
    
    // 显示班级学生
    printf("\n=== 班级学生 ===\n");
    bool found = false;
    
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].class_id, id) == 0) {
            printf("学号: %s\n", students[i].id);
            printf("姓名: %s\n", students[i].name);
            printf("性别: %s\n", students[i].gender == MALE ? "男" : 
                   students[i].gender == FEMALE ? "女" : "其他");
            printf("年级: %s\n", students[i].year == FRESHMAN ? "大一" : 
                   students[i].year == SOPHOMORE ? "大二" : 
                   students[i].year == JUNIOR ? "大三" : 
                   students[i].year == SENIOR ? "大四" : "研究生");
            printf("GPA: %.2f\n", students[i].gpa);
            printf("------------------------\n");
            found = true;
        }
    }
    
    if (!found) {
        printf("该班级没有学生。\n");
    }
}

// 列出所有班级
void list_all_classes() {
    printf("\n=== 所有班级列表 ===\n");
    
    if (class_count == 0) {
        printf("没有班级记录。\n");
        return;
    }
    
    printf("%-15s %-20s %-15s %-15s %-15s %-15s\n", 
           "班级ID", "班级名称", "院系", "教师ID", "学生数", "最大学生数");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < class_count; i++) {
        printf("%-15s %-20s %-15s %-15s %-15d %-15d\n", 
               classes[i].id, 
               classes[i].name, 
               classes[i].department, 
               classes[i].teacher_id, 
               classes[i].student_count, 
               classes[i].max_students);
    }
}

// 分配教师到班级
void assign_teacher_to_class() {
    printf("\n=== 分配教师到班级 ===\n");
    printf("请输入班级ID: ");
    char class_id[MAX_ID_LEN];
    get_string_input("", class_id, MAX_ID_LEN);
    
    // 查找班级
    int class_index = -1;
    for (int i = 0; i < class_count; i++) {
        if (strcmp(classes[i].id, class_id) == 0) {
            class_index = i;
            break;
        }
    }
    
    if (class_index == -1) {
        printf("未找到该班级。\n");
        return;
    }
    
    printf("当前教师ID: %s\n", classes[class_index].teacher_id);
    printf("请输入新的教师ID (留空则取消分配): ");
    char teacher_id[MAX_ID_LEN];
    get_string_input("", teacher_id, MAX_ID_LEN);
    
    if (strlen(teacher_id) == 0) {
        // 取消分配
        strcpy(classes[class_index].teacher_id, "");
        printf("已取消教师分配。\n");
        return;
    }
    
    // 检查教师是否存在
    bool teacher_exists = false;
    for (int i = 0; i < teacher_count; i++) {
        if (strcmp(teachers[i].id, teacher_id) == 0 && teachers[i].is_active) {
            teacher_exists = true;
            break;
        }
    }
    
    if (!teacher_exists) {
        printf("教师ID不存在或教师已离职。\n");
        return;
    }
    
    // 分配教师
    strcpy(classes[class_index].teacher_id, teacher_id);
    printf("教师分配成功！\n");
}

// 分配学生到班级
void assign_student_to_class() {
    printf("\n=== 分配学生到班级 ===\n");
    printf("请输入学生学号: ");
    char student_id[MAX_ID_LEN];
    get_string_input("", student_id, MAX_ID_LEN);
    
    // 查找学生
    int student_index = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, student_id) == 0) {
            student_index = i;
            break;
        }
    }
    
    if (student_index == -1) {
        printf("未找到该学生。\n");
        return;
    }
    
    printf("当前班级ID: %s\n", students[student_index].class_id);
    printf("请输入新的班级ID (留空则取消分配): ");
    char class_id[MAX_ID_LEN];
    get_string_input("", class_id, MAX_ID_LEN);
    
    if (strlen(class_id) == 0) {
        // 取消分配
        // 更新原班级的学生数
        if (strlen(students[student_index].class_id) > 0) {
            for (int i = 0; i < class_count; i++) {
                if (strcmp(classes[i].id, students[student_index].class_id) == 0) {
                    classes[i].student_count--;
                    break;
                }
            }
        }
        
        strcpy(students[student_index].class_id, "");
        printf("已取消班级分配。\n");
        return;
    }
    
    // 检查班级是否存在
    int class_index = -1;
    for (int i = 0; i < class_count; i++) {
        if (strcmp(classes[i].id, class_id) == 0) {
            class_index = i;
            break;
        }
    }
    
    if (class_index == -1) {
        printf("班级ID不存在。\n");
        return;
    }
    
    // 检查班级是否已满
    if (classes[class_index].student_count >= classes[class_index].max_students) {
        printf("该班级已满员。\n");
        return;
    }
    
    // 更新原班级的学生数
    if (strlen(students[student_index].class_id) > 0) {
        for (int i = 0; i < class_count; i++) {
            if (strcmp(classes[i].id, students[student_index].class_id) == 0) {
                classes[i].student_count--;
                break;
            }
        }
    }
    
    // 分配学生到新班级
    strcpy(students[student_index].class_id, class_id);
    classes[class_index].student_count++;
    
    printf("学生分配成功！\n");
}

// 生成学生报告
void generate_student_report() {
    printf("\n=== 生成学生报告 ===\n");
    printf("请输入学生学号 (留空则生成所有学生报告): ");
    char student_id[MAX_ID_LEN];
    get_string_input("", student_id, MAX_ID_LEN);
    
    if (strlen(student_id) > 0) {
        // 生成单个学生报告
        int student_index = -1;
        for (int i = 0; i < student_count; i++) {
            if (strcmp(students[i].id, student_id) == 0) {
                student_index = i;
                break;
            }
        }
        
        if (student_index == -1) {
            printf("未找到该学生。\n");
            return;
        }
        
        printf("\n=== 学生报告: %s ===\n", students[student_index].name);
        printf("学号: %s\n", students[student_index].id);
        printf("姓名: %s\n", students[student_index].name);
        printf("性别: %s\n", students[student_index].gender == MALE ? "男" : 
               students[student_index].gender == FEMALE ? "女" : "其他");
        printf("出生日期: %d-%d-%d\n", students[student_index].birth_date.year, 
               students[student_index].birth_date.month, students[student_index].birth_date.day);
        printf("院系: %s\n", students[student_index].department);
        printf("年级: %s\n", students[student_index].year == FRESHMAN ? "大一" : 
               students[student_index].year == SOPHOMORE ? "大二" : 
               students[student_index].year == JUNIOR ? "大三" : 
               students[student_index].year == SENIOR ? "大四" : "研究生");
        printf("GPA: %.2f\n", students[student_index].gpa);
        printf("总学分: %d\n", students[student_index].total_credits);
        printf("入学日期: %d-%d-%d\n", students[student_index].admission_date.year, 
               students[student_index].admission_date.month, students[student_index].admission_date.day);
        printf("状态: %s\n", students[student_index].is_active ? "在读" : "休学/毕业");
        printf("班级: %s\n", students[student_index].class_id);
        
        // 统计选课情况
        int total_courses = 0;
        int completed_courses = 0;
        float total_score = 0.0;
        int grade_counts[5] = {0}; // A, B, C, D, F
        
        for (int i = 0; i < enrollment_count; i++) {
            if (strcmp(enrollments[i].student_id, student_id) == 0) {
                total_courses++;
                
                if (enrollments[i].completed) {
                    completed_courses++;
                    total_score += enrollments[i].average;
                    
                    // 统计等级
                    if (enrollments[i].average >= 90) grade_counts[0]++;
                    else if (enrollments[i].average >= 80) grade_counts[1]++;
                    else if (enrollments[i].average >= 70) grade_counts[2]++;
                    else if (enrollments[i].average >= 60) grade_counts[3]++;
                    else grade_counts[4]++;
                }
            }
        }
        
        printf("\n=== 选课统计 ===\n");
        printf("总选课数: %d\n", total_courses);
        printf("已完成课程数: %d\n", completed_courses);
        printf("进行中课程数: %d\n", total_courses - completed_courses);
        
        if (completed_courses > 0) {
            printf("平均成绩: %.2f\n", total_score / completed_courses);
            printf("等级分布:\n");
            printf("  A (90-100): %d (%.1f%%)\n", grade_counts[0], 
                   (float)grade_counts[0] / completed_courses * 100);
            printf("  B (80-89): %d (%.1f%%)\n", grade_counts[1], 
                   (float)grade_counts[1] / completed_courses * 100);
            printf("  C (70-79): %d (%.1f%%)\n", grade_counts[2], 
                   (float)grade_counts[2] / completed_courses * 100);
            printf("  D (60-69): %d (%.1f%%)\n", grade_counts[3], 
                   (float)grade_counts[3] / completed_courses * 100);
            printf("  F (0-59): %d (%.1f%%)\n", grade_counts[4], 
                   (float)grade_counts[4] / completed_courses * 100);
        } else {
            printf("没有已完成课程的成绩数据。\n");
        }
    } else {
        // 生成所有学生报告
        printf("\n=== 所有学生报告 ===\n");
        
        if (student_count == 0) {
            printf("没有学生记录。\n");
            return;
        }
        
        // 统计总体数据
        int total_students = student_count;
        int active_students = 0;
        float total_gpa = 0.0;
        int total_credits = 0;
        int grade_counts[5] = {0}; // A, B, C, D, F
        int total_grades = 0;
        
        for (int i = 0; i < student_count; i++) {
            if (students[i].is_active) {
                active_students++;
            }
            
            total_gpa += students[i].gpa;
            total_credits += students[i].total_credits;
            
            // 统计成绩等级
            for (int j = 0; j < enrollment_count; j++) {
                if (strcmp(enrollments[j].student_id, students[i].id) == 0 && 
                    enrollments[j].completed) {
                    total_grades++;
                    
                    if (enrollments[j].average >= 90) grade_counts[0]++;
                    else if (enrollments[j].average >= 80) grade_counts[1]++;
                    else if (enrollments[j].average >= 70) grade_counts[2]++;
                    else if (enrollments[j].average >= 60) grade_counts[3]++;
                    else grade_counts[4]++;
                }
            }
        }
        
        printf("学生总数: %d\n", total_students);
        printf("在读学生数: %d\n", active_students);
        printf("休学/毕业学生数: %d\n", total_students - active_students);
        printf("平均GPA: %.2f\n", total_gpa / total_students);
        printf("总学分: %d\n", total_credits);
        
        if (total_grades > 0) {
            printf("\n=== 成绩等级分布 ===\n");
            printf("A (90-100): %d (%.1f%%)\n", grade_counts[0], 
                   (float)grade_counts[0] / total_grades * 100);
            printf("B (80-89): %d (%.1f%%)\n", grade_counts[1], 
                   (float)grade_counts[1] / total_grades * 100);
            printf("C (70-79): %d (%.1f%%)\n", grade_counts[2], 
                   (float)grade_counts[2] / total_grades * 100);
            printf("D (60-69): %d (%.1f%%)\n", grade_counts[3], 
                   (float)grade_counts[3] / total_grades * 100);
            printf("F (0-59): %d (%.1f%%)\n", grade_counts[4], 
                   (float)grade_counts[4] / total_grades * 100);
        } else {
            printf("\n没有成绩数据。\n");
        }
        
        // 按院系统计
        printf("\n=== 院系统计 ===\n");
        char departments[MAX_DEPARTMENT_LEN][MAX_DEPARTMENT_LEN];
        int dept_count = 0;
        
        // 收集所有院系
        for (int i = 0; i < student_count; i++) {
            bool found = false;
            for (int j = 0; j < dept_count; j++) {
                if (strcmp(students[i].department, departments[j]) == 0) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                strcpy(departments[dept_count], students[i].department);
                dept_count++;
            }
        }
        
        for (int i = 0; i < dept_count; i++) {
            int dept_students = 0;
            int dept_active = 0;
            float dept_gpa = 0.0;
            int dept_credits = 0;
            
            for (int j = 0; j < student_count; j++) {
                if (strcmp(students[j].department, departments[i]) == 0) {
                    dept_students++;
                    if (students[j].is_active) {
                        dept_active++;
                    }
                    dept_gpa += students[j].gpa;
                    dept_credits += students[j].total_credits;
                }
            }
            
            printf("\n%s:\n", departments[i]);
            printf("  学生数: %d\n", dept_students);
            printf("  在读学生数: %d\n", dept_active);
            printf("  平均GPA: %.2f\n", dept_gpa / dept_students);
            printf("  总学分: %d\n", dept_credits);
        }
    }
}

// 生成课程报告
void generate_course_report() {
    printf("\n=== 生成课程报告 ===\n");
    printf("请输入课程ID (留空则生成所有课程报告): ");
    char course_id[MAX_ID_LEN];
    get_string_input("", course_id, MAX_ID_LEN);
    
    if (strlen(course_id) > 0) {
        // 生成单个课程报告
        int course_index = -1;
        for (int i = 0; i < course_count; i++) {
            if (strcmp(courses[i].course_id, course_id) == 0) {
                course_index = i;
                break;
            }
        }
        
        if (course_index == -1) {
            printf("未找到该课程。\n");
            return;
        }
        
        printf("\n=== 课程报告: %s ===\n", courses[course_index].name);
        printf("课程ID: %s\n", courses[course_index].course_id);
        printf("课程名称: %s\n", courses[course_index].name);
        printf("学分: %d\n", courses[course_index].credits);
        printf("院系: %s\n", courses[course_index].department);
        printf("教师ID: %s\n", courses[course_index].teacher_id);
        printf("学期: %s\n", courses[course_index].semester == SPRING ? "春季" : 
               courses[course_index].semester == SUMMER ? "夏季" : 
               courses[course_index].semester == FALL ? "秋季" : "冬季");
        printf("年份: %d\n", courses[course_index].year);
        printf("最大学生数: %d\n", courses[course_index].max_students);
        printf("当前学生数: %d\n", courses[course_index].current_students);
        
        // 统计学生成绩
        int total_students = 0;
        float total_score = 0.0;
        int grade_counts[5] = {0}; // A, B, C, D, F
        int completed_students = 0;
        
        for (int i = 0; i < enrollment_count; i++) {
            if (strcmp(enrollments[i].course_id, course_id) == 0) {
                total_students++;
                
                if (enrollments[i].completed) {
                    completed_students++;
                    total_score += enrollments[i].average;
                    
                    // 统计等级
                    if (enrollments[i].average >= 90) grade_counts[0]++;
                    else if (enrollments[i].average >= 80) grade_counts[1]++;
                    else if (enrollments[i].average >= 70) grade_counts[2]++;
                    else if (enrollments[i].average >= 60) grade_counts[3]++;
                    else grade_counts[4]++;
                }
            }
        }
        
        printf("\n=== 学生统计 ===\n");
        printf("选课学生数: %d\n", total_students);
        printf("完成课程学生数: %d\n", completed_students);
        printf("未完成课程学生数: %d\n", total_students - completed_students);
        
        if (completed_students > 0) {
            printf("平均成绩: %.2f\n", total_score / completed_students);
            printf("等级分布:\n");
            printf("  A (90-100): %d (%.1f%%)\n", grade_counts[0], 
                   (float)grade_counts[0] / completed_students * 100);
            printf("  B (80-89): %d (%.1f%%)\n", grade_counts[1], 
                   (float)grade_counts[1] / completed_students * 100);
            printf("  C (70-79): %d (%.1f%%)\n", grade_counts[2], 
                   (float)grade_counts[2] / completed_students * 100);
            printf("  D (60-69): %d (%.1f%%)\n", grade_counts[3], 
                   (float)grade_counts[3] / completed_students * 100);
            printf("  F (0-59): %d (%.1f%%)\n", grade_counts[4], 
                   (float)grade_counts[4] / completed_students * 100);
        } else {
            printf("没有已完成课程的成绩数据。\n");
        }
    } else {
        // 生成所有课程报告
        printf("\n=== 所有课程报告 ===\n");
        
        if (course_count == 0) {
            printf("没有课程记录。\n");
            return;
        }
        
        // 统计总体数据
        int total_courses = course_count;
        int total_enrollments = 0;
        int total_completed = 0;
        float total_score = 0.0;
        int grade_counts[5] = {0}; // A, B, C, D, F
        int total_grades = 0;
        
        for (int i = 0; i < course_count; i++) {
            total_enrollments += courses[i].current_students;
            
            for (int j = 0; j < enrollment_count; j++) {
                if (strcmp(enrollments[j].course_id, courses[i].course_id) == 0) {
                    if (enrollments[j].completed) {
                        total_completed++;
                        total_score += enrollments[j].average;
                        total_grades++;
                        
                        // 统计等级
                        if (enrollments[j].average >= 90) grade_counts[0]++;
                        else if (enrollments[j].average >= 80) grade_counts[1]++;
                        else if (enrollments[j].average >= 70) grade_counts[2]++;
                        else if (enrollments[j].average >= 60) grade_counts[3]++;
                        else grade_counts[4]++;
                    }
                }
            }
        }
        
        printf("课程总数: %d\n", total_courses);
        printf("总选课人次: %d\n", total_enrollments);
        printf("完成课程人次: %d\n", total_completed);
        printf("未完成课程人次: %d\n", total_enrollments - total_completed);
        
        if (total_completed > 0) {
            printf("平均成绩: %.2f\n", total_score / total_completed);
        }
        
        if (total_grades > 0) {
            printf("\n=== 成绩等级分布 ===\n");
            printf("A (90-100): %d (%.1f%%)\n", grade_counts[0], 
                   (float)grade_counts[0] / total_grades * 100);
            printf("B (80-89): %d (%.1f%%)\n", grade_counts[1], 
                   (float)grade_counts[1] / total_grades * 100);
            printf("C (70-79): %d (%.1f%%)\n", grade_counts[2], 
                   (float)grade_counts[2] / total_grades * 100);
            printf("D (60-69): %d (%.1f%%)\n", grade_counts[3], 
                   (float)grade_counts[3] / total_grades * 100);
            printf("F (0-59): %d (%.1f%%)\n", grade_counts[4], 
                   (float)grade_counts[4] / total_grades * 100);
        } else {
            printf("\n没有成绩数据。\n");
        }
        
        // 按院系统计
        printf("\n=== 院系统计 ===\n");
        char departments[MAX_DEPARTMENT_LEN][MAX_DEPARTMENT_LEN];
        int dept_count = 0;
        
        // 收集所有院系
        for (int i = 0; i < course_count; i++) {
            bool found = false;
            for (int j = 0; j < dept_count; j++) {
                if (strcmp(courses[i].department, departments[j]) == 0) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                strcpy(departments[dept_count], courses[i].department);
                dept_count++;
            }
        }
        
        for (int i = 0; i < dept_count; i++) {
            int dept_courses = 0;
            int dept_enrollments = 0;
            int dept_completed = 0;
            float dept_score = 0.0;
            
            for (int j = 0; j < course_count; j++) {
                if (strcmp(courses[j].department, departments[i]) == 0) {
                    dept_courses++;
                    dept_enrollments += courses[j].current_students;
                    
                    for (int k = 0; k < enrollment_count; k++) {
                        if (strcmp(enrollments[k].course_id, courses[j].course_id) == 0) {
                            if (enrollments[k].completed) {
                                dept_completed++;
                                dept_score += enrollments[k].average;
                            }
                        }
                    }
                }
            }
            
            printf("\n%s:\n", departments[i]);
            printf("  课程数: %d\n", dept_courses);
            printf("  选课人次: %d\n", dept_enrollments);
            printf("  完成人次: %d\n", dept_completed);
            if (dept_completed > 0) {
                printf("  平均成绩: %.2f\n", dept_score / dept_completed);
            }
        }
    }
}

// 生成教师报告
void generate_teacher_report() {
    printf("\n=== 生成教师报告 ===\n");
    printf("请输入教师ID (留空则生成所有教师报告): ");
    char teacher_id[MAX_ID_LEN];
    get_string_input("", teacher_id, MAX_ID_LEN);
    
    if (strlen(teacher_id) > 0) {
        // 生成单个教师报告
        int teacher_index = -1;
        for (int i = 0; i < teacher_count; i++) {
            if (strcmp(teachers[i].id, teacher_id) == 0) {
                teacher_index = i;
                break;
            }
        }
        
        if (teacher_index == -1) {
            printf("未找到该教师。\n");
            return;
        }
        
        printf("\n=== 教师报告: %s ===\n", teachers[teacher_index].name);
        printf("教师ID: %s\n", teachers[teacher_index].id);
        printf("姓名: %s\n", teachers[teacher_index].name);
        printf("性别: %s\n", teachers[teacher_index].gender == MALE ? "男" : 
               teachers[teacher_index].gender == FEMALE ? "女" : "其他");
        printf("院系: %s\n", teachers[teacher_index].department);
        printf("职称: %s\n", teachers[teacher_index].title);
        printf("聘用日期: %d-%d-%d\n", teachers[teacher_index].hire_date.year, 
               teachers[teacher_index].hire_date.month, teachers[teacher_index].hire_date.day);
        printf("薪资: %.2f\n", teachers[teacher_index].salary);
        printf("状态: %s\n", teachers[teacher_index].is_active ? "在职" : "离职");
        
        // 统计教授的课程
        int teaching_courses = 0;
        int total_students = 0;
        float total_score = 0.0;
        int grade_counts[5] = {0}; // A, B, C, D, F
        int total_grades = 0;
        
        for (int i = 0; i < course_count; i++) {
            if (strcmp(courses[i].teacher_id, teacher_id) == 0) {
                teaching_courses++;
                total_students += courses[i].current_students;
                
                for (int j = 0; j < enrollment_count; j++) {
                    if (strcmp(enrollments[j].course_id, courses[i].course_id) == 0 && 
                        enrollments[j].completed) {
                        total_score += enrollments[j].average;
                        total_grades++;
                        
                        // 统计等级
                        if (enrollments[j].average >= 90) grade_counts[0]++;
                        else if (enrollments[j].average >= 80) grade_counts[1]++;
                        else if (enrollments[j].average >= 70) grade_counts[2]++;
                        else if (enrollments[j].average >= 60) grade_counts[3]++;
                        else grade_counts[4]++;
                    }
                }
            }
        }
        
        printf("\n=== 教学统计 ===\n");
        printf("教授课程数: %d\n", teaching_courses);
        printf("总学生数: %d\n", total_students);
        
        if (total_grades > 0) {
            printf("平均成绩: %.2f\n", total_score / total_grades);
            printf("等级分布:\n");
            printf("  A (90-100): %d (%.1f%%)\n", grade_counts[0], 
                   (float)grade_counts[0] / total_grades * 100);
            printf("  B (80-89): %d (%.1f%%)\n", grade_counts[1], 
                   (float)grade_counts[1] / total_grades * 100);
            printf("  C (70-79): %d (%.1f%%)\n", grade_counts[2], 
                   (float)grade_counts[2] / total_grades * 100);
            printf("  D (60-69): %d (%.1f%%)\n", grade_counts[3], 
                   (float)grade_counts[3] / total_grades * 100);
            printf("  F (0-59): %d (%.1f%%)\n", grade_counts[4], 
                   (float)grade_counts[4] / total_grades * 100);
        } else {
            printf("没有成绩数据。\n");
        }
        
        // 统计负责的班级
        int managing_classes = 0;
        int class_students = 0;
        
        for (int i = 0; i < class_count; i++) {
            if (strcmp(classes[i].teacher_id, teacher_id) == 0) {
                managing_classes++;
                class_students += classes[i].student_count;
            }
        }
        
        printf("\n=== 班级管理 ===\n");
        printf("负责班级数: %d\n", managing_classes);
        printf("班级学生数: %d\n", class_students);
    } else {
        // 生成所有教师报告
        printf("\n=== 所有教师报告 ===\n");
        
        if (teacher_count == 0) {
            printf("没有教师记录。\n");
            return;
        }
        
        // 统计总体数据
        int total_teachers = teacher_count;
        int active_teachers = 0;
        float total_salary = 0.0;
        int teaching_courses = 0;
        int total_students = 0;
        float total_score = 0.0;
        int grade_counts[5] = {0}; // A, B, C, D, F
        int total_grades = 0;
        
        for (int i = 0; i < teacher_count; i++) {
            if (teachers[i].is_active) {
                active_teachers++;
            }
            
            total_salary += teachers[i].salary;
            
            for (int j = 0; j < course_count; j++) {
                if (strcmp(courses[j].teacher_id, teachers[i].id) == 0) {
                    teaching_courses++;
                    total_students += courses[j].current_students;
                    
                    for (int k = 0; k < enrollment_count; k++) {
                        if (strcmp(enrollments[k].course_id, courses[j].course_id) == 0 && 
                            enrollments[k].completed) {
                            total_score += enrollments[k].average;
                            total_grades++;
                            
                            // 统计等级
                            if (enrollments[k].average >= 90) grade_counts[0]++;
                            else if (enrollments[k].average >= 80) grade_counts[1]++;
                            else if (enrollments[k].average >= 70) grade_counts[2]++;
                            else if (enrollments[k].average >= 60) grade_counts[3]++;
                            else grade_counts[4]++;
                        }
                    }
                }
            }
        }
        
        printf("教师总数: %d\n", total_teachers);
        printf("在职教师数: %d\n", active_teachers);
        printf("离职教师数: %d\n", total_teachers - active_teachers);
        printf("平均薪资: %.2f\n", total_salary / total_teachers);
        printf("总课程数: %d\n", teaching_courses);
        printf("总学生数: %d\n", total_students);
        
        if (total_grades > 0) {
            printf("平均成绩: %.2f\n", total_score / total_grades);
        }
        
        if (total_grades > 0) {
            printf("\n=== 成绩等级分布 ===\n");
            printf("A (90-100): %d (%.1f%%)\n", grade_counts[0], 
                   (float)grade_counts[0] / total_grades * 100);
            printf("B (80-89): %d (%.1f%%)\n", grade_counts[1], 
                   (float)grade_counts[1] / total_grades * 100);
            printf("C (70-79): %d (%.1f%%)\n", grade_counts[2], 
                   (float)grade_counts[2] / total_grades * 100);
            printf("D (60-69): %d (%.1f%%)\n", grade_counts[3], 
                   (float)grade_counts[3] / total_grades * 100);
            printf("F (0-59): %d (%.1f%%)\n", grade_counts[4], 
                   (float)grade_counts[4] / total_grades * 100);
        } else {
            printf("\n没有成绩数据。\n");
        }
        
        // 按院系统计
        printf("\n=== 院系统计 ===\n");
        char departments[MAX_DEPARTMENT_LEN][MAX_DEPARTMENT_LEN];
        int dept_count = 0;
        
        // 收集所有院系
        for (int i = 0; i < teacher_count; i++) {
            bool found = false;
            for (int j = 0; j < dept_count; j++) {
                if (strcmp(teachers[i].department, departments[j]) == 0) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                strcpy(departments[dept_count], teachers[i].department);
                dept_count++;
            }
        }
        
        for (int i = 0; i < dept_count; i++) {
            int dept_teachers = 0;
            int dept_active = 0;
            float dept_salary = 0.0;
            int dept_courses = 0;
            int dept_students = 0;
            
            for (int j = 0; j < teacher_count; j++) {
                if (strcmp(teachers[j].department, departments[i]) == 0) {
                    dept_teachers++;
                    if (teachers[j].is_active) {
                        dept_active++;
                    }
                    dept_salary += teachers[j].salary;
                    
                    for (int k = 0; k < course_count; k++) {
                        if (strcmp(courses[k].teacher_id, teachers[j].id) == 0) {
                            dept_courses++;
                            dept_students += courses[k].current_students;
                        }
                    }
                }
            }
            
            printf("\n%s:\n", departments[i]);
            printf("  教师数: %d\n", dept_teachers);
            printf("  在职教师数: %d\n", dept_active);
            printf("  平均薪资: %.2f\n", dept_salary / dept_teachers);
            printf("  课程数: %d\n", dept_courses);
            printf("  学生数: %d\n", dept_students);
        }
    }
}

// 生成院系报告
void generate_department_report() {
    printf("\n=== 生成院系报告 ===\n");
    printf("请输入院系名称 (留空则生成所有院系报告): ");
    char department[MAX_DEPARTMENT_LEN];
    get_string_input("", department, MAX_DEPARTMENT_LEN);
    
    if (strlen(department) > 0) {
        // 生成单个院系报告
        bool dept_exists = false;
        
        // 检查院系是否存在
        for (int i = 0; i < student_count; i++) {
            if (strcmp(students[i].department, department) == 0) {
                dept_exists = true;
                break;
            }
        }
        
        if (!dept_exists) {
            for (int i = 0; i < teacher_count; i++) {
                if (strcmp(teachers[i].department, department) == 0) {
                    dept_exists = true;
                    break;
                }
            }
        }
        
        if (!dept_exists) {
            for (int i = 0; i < course_count; i++) {
                if (strcmp(courses[i].department, department) == 0) {
                    dept_exists = true;
                    break;
                }
            }
        }
        
        if (!dept_exists) {
            printf("未找到该院系。\n");
            return;
        }
        
        printf("\n=== 院系报告: %s ===\n", department);
        
        // 统计学生数据
        int dept_students = 0;
        int dept_active_students = 0;
        float dept_gpa = 0.0;
        int dept_credits = 0;
        
        for (int i = 0; i < student_count; i++) {
            if (strcmp(students[i].department, department) == 0) {
                dept_students++;
                if (students[i].is_active) {
                    dept_active_students++;
                }
                dept_gpa += students[i].gpa;
                dept_credits += students[i].total_credits;
            }
        }
        
        printf("\n=== 学生统计 ===\n");
        printf("学生数: %d\n", dept_students);
        printf("在读学生数: %d\n", dept_active_students);
        printf("休学/毕业学生数: %d\n", dept_students - dept_active_students);
        if (dept_students > 0) {
            printf("平均GPA: %.2f\n", dept_gpa / dept_students);
            printf("总学分: %d\n", dept_credits);
        }
        
        // 统计教师数据
        int dept_teachers = 0;
        int dept_active_teachers = 0;
        float dept_salary = 0.0;
        
        for (int i = 0; i < teacher_count; i++) {
            if (strcmp(teachers[i].department, department) == 0) {
                dept_teachers++;
                if (teachers[i].is_active) {
                    dept_active_teachers++;
                }
                dept_salary += teachers[i].salary;
            }
        }
        
        printf("\n=== 教师统计 ===\n");
        printf("教师数: %d\n", dept_teachers);
        printf("在职教师数: %d\n", dept_active_teachers);
        printf("离职教师数: %d\n", dept_teachers - dept_active_teachers);
        if (dept_teachers > 0) {
            printf("平均薪资: %.2f\n", dept_salary / dept_teachers);
        }
        
        // 统计课程数据
        int dept_courses = 0;
        int dept_enrollments = 0;
        float dept_score = 0.0;
        int grade_counts[5] = {0}; // A, B, C, D, F
        int total_grades = 0;
        
        for (int i = 0; i < course_count; i++) {
            if (strcmp(courses[i].department, department) == 0) {
                dept_courses++;
                dept_enrollments += courses[i].current_students;
                
                for (int j = 0; j < enrollment_count; j++) {
                    if (strcmp(enrollments[j].course_id, courses[i].course_id) == 0 && 
                        enrollments[j].completed) {
                        dept_score += enrollments[j].average;
                        total_grades++;
                        
                        // 统计等级
                        if (enrollments[j].average >= 90) grade_counts[0]++;
                        else if (enrollments[j].average >= 80) grade_counts[1]++;
                        else if (enrollments[j].average >= 70) grade_counts[2]++;
                        else if (enrollments[j].average >= 60) grade_counts[3]++;
                        else grade_counts[4]++;
                    }
                }
            }
        }
        
        printf("\n=== 课程统计 ===\n");
        printf("课程数: %d\n", dept_courses);
        printf("选课人次: %d\n", dept_enrollments);
        if (total_grades > 0) {
            printf("平均成绩: %.2f\n", dept_score / total_grades);
            printf("等级分布:\n");
            printf("  A (90-100): %d (%.1f%%)\n", grade_counts[0], 
                   (float)grade_counts[0] / total_grades * 100);
            printf("  B (80-89): %d (%.1f%%)\n", grade_counts[1], 
                   (float)grade_counts[1] / total_grades * 100);
            printf("  C (70-79): %d (%.1f%%)\n", grade_counts[2], 
                   (float)grade_counts[2] / total_grades * 100);
            printf("  D (60-69): %d (%.1f%%)\n", grade_counts[3], 
                   (float)grade_counts[3] / total_grades * 100);
            printf("  F (0-59): %d (%.1f%%)\n", grade_counts[4], 
                   (float)grade_counts[4] / total_grades * 100);
        } else {
            printf("没有成绩数据。\n");
        }
    } else {
        // 生成所有院系报告
        printf("\n=== 所有院系报告 ===\n");
        
        // 收集所有院系
        char departments[MAX_DEPARTMENT_LEN][MAX_DEPARTMENT_LEN];
        int dept_count = 0;
        
        for (int i = 0; i < student_count; i++) {
            bool found = false;
            for (int j = 0; j < dept_count; j++) {
                if (strcmp(students[i].department, departments[j]) == 0) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                strcpy(departments[dept_count], students[i].department);
                dept_count++;
            }
        }
        
        for (int i = 0; i < teacher_count; i++) {
            bool found = false;
            for (int j = 0; j < dept_count; j++) {
                if (strcmp(teachers[i].department, departments[j]) == 0) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                strcpy(departments[dept_count], teachers[i].department);
                dept_count++;
            }
        }
        
        for (int i = 0; i < course_count; i++) {
            bool found = false;
            for (int j = 0; j < dept_count; j++) {
                if (strcmp(courses[i].department, departments[j]) == 0) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                strcpy(departments[dept_count], courses[i].department);
                dept_count++;
            }
        }
        
        if (dept_count == 0) {
            printf("没有院系记录。\n");
            return;
        }
        
        // 统计总体数据
        int total_students = 0;
        int total_active_students = 0;
        float total_gpa = 0.0;
        int total_credits = 0;
        int total_teachers = 0;
        int total_active_teachers = 0;
        float total_salary = 0.0;
        int total_courses = 0;
        int total_enrollments = 0;
        float total_score = 0.0;
        int grade_counts[5] = {0}; // A, B, C, D, F
        int total_grades = 0;
        
        for (int i = 0; i < dept_count; i++) {
            for (int j = 0; j < student_count; j++) {
                if (strcmp(students[j].department, departments[i]) == 0) {
                    total_students++;
                    if (students[j].is_active) {
                        total_active_students++;
                    }
                    total_gpa += students[j].gpa;
                    total_credits += students[j].total_credits;
                }
            }
            
            for (int j = 0; j < teacher_count; j++) {
                if (strcmp(teachers[j].department, departments[i]) == 0) {
                    total_teachers++;
                    if (teachers[j].is_active) {
                        total_active_teachers++;
                    }
                    total_salary += teachers[j].salary;
                }
            }
            
            for (int j = 0; j < course_count; j++) {
                if (strcmp(courses[j].department, departments[i]) == 0) {
                    total_courses++;
                    total_enrollments += courses[j].current_students;
                    
                    for (int k = 0; k < enrollment_count; k++) {
                        if (strcmp(enrollments[k].course_id, courses[j].course_id) == 0 && 
                            enrollments[k].completed) {
                            total_score += enrollments[k].average;
                            total_grades++;
                            
                            // 统计等级
                            if (enrollments[k].average >= 90) grade_counts[0]++;
                            else if (enrollments[k].average >= 80) grade_counts[1]++;
                            else if (enrollments[k].average >= 70) grade_counts[2]++;
                            else if (enrollments[k].average >= 60) grade_counts[3]++;
                            else grade_counts[4]++;
                        }
                    }
                }
            }
        }
        
        printf("院系数: %d\n", dept_count);
        printf("学生总数: %d\n", total_students);
        printf("在读学生数: %d\n", total_active_students);
        printf("教师总数: %d\n", total_teachers);
        printf("在职教师数: %d\n", total_active_teachers);
        printf("课程总数: %d\n", total_courses);
        printf("选课总人次: %d\n", total_enrollments);
        
        if (total_students > 0) {
            printf("平均GPA: %.2f\n", total_gpa / total_students);
        }
        
        if (total_teachers > 0) {
            printf("平均薪资: %.2f\n", total_salary / total_teachers);
        }
        
        if (total_grades > 0) {
            printf("平均成绩: %.2f\n", total_score / total_grades);
        }
        
        if (total_grades > 0) {
            printf("\n=== 成绩等级分布 ===\n");
            printf("A (90-100): %d (%.1f%%)\n", grade_counts[0], 
                   (float)grade_counts[0] / total_grades * 100);
            printf("B (80-89): %d (%.1f%%)\n", grade_counts[1], 
                   (float)grade_counts[1] / total_grades * 100);
            printf("C (70-79): %d (%.1f%%)\n", grade_counts[2], 
                   (float)grade_counts[2] / total_grades * 100);
            printf("D (60-69): %d (%.1f%%)\n", grade_counts[3], 
                   (float)grade_counts[3] / total_grades * 100);
            printf("F (0-59): %d (%.1f%%)\n", grade_counts[4], 
                   (float)grade_counts[4] / total_grades * 100);
        } else {
            printf("\n没有成绩数据。\n");
        }
        
        // 显示各院系详细信息
        printf("\n=== 各院系详细信息 ===\n");
        for (int i = 0; i < dept_count; i++) {
            int dept_students = 0;
            int dept_active_students = 0;
            float dept_gpa = 0.0;
            int dept_credits = 0;
            int dept_teachers = 0;
            int dept_active_teachers = 0;
            float dept_salary = 0.0;
            int dept_courses = 0;
            int dept_enrollments = 0;
            float dept_score = 0.0;
            int dept_grades = 0;
            
            for (int j = 0; j < student_count; j++) {
                if (strcmp(students[j].department, departments[i]) == 0) {
                    dept_students++;
                    if (students[j].is_active) {
                        dept_active_students++;
                    }
                    dept_gpa += students[j].gpa;
                    dept_credits += students[j].total_credits;
                }
            }
            
            for (int j = 0; j < teacher_count; j++) {
                if (strcmp(teachers[j].department, departments[i]) == 0) {
                    dept_teachers++;
                    if (teachers[j].is_active) {
                        dept_active_teachers++;
                    }
                    dept_salary += teachers[j].salary;
                }
            }
            
            for (int j = 0; j < course_count; j++) {
                if (strcmp(courses[j].department, departments[i]) == 0) {
                    dept_courses++;
                    dept_enrollments += courses[j].current_students;
                    
                    for (int k = 0; k < enrollment_count; k++) {
                        if (strcmp(enrollments[k].course_id, courses[j].course_id) == 0 && 
                            enrollments[k].completed) {
                            dept_score += enrollments[k].average;
                            dept_grades++;
                        }
                    }
                }
            }
            
            printf("\n%s:\n", departments[i]);
            printf("  学生数: %d (在读: %d)\n", dept_students, dept_active_students);
            printf("  教师数: %d (在职: %d)\n", dept_teachers, dept_active_teachers);
            printf("  课程数: %d\n", dept_courses);
            printf("  选课人次: %d\n", dept_enrollments);
            if (dept_students > 0) {
                printf("  平均GPA: %.2f\n", dept_gpa / dept_students);
            }
            if (dept_teachers > 0) {
                printf("  平均薪资: %.2f\n", dept_salary / dept_teachers);
            }
            if (dept_grades > 0) {
                printf("  平均成绩: %.2f\n", dept_score / dept_grades);
            }
        }
    }
}

// 生成统计信息
void generate_statistics() {
    printf("\n=== 系统统计信息 ===\n");
    
    // 基本统计
    printf("\n=== 基本统计 ===\n");
    printf("学生总数: %d\n", student_count);
    printf("教师总数: %d\n", teacher_count);
    printf("课程总数: %d\n", course_count);
    printf("班级总数: %d\n", class_count);
    printf("选课记录总数: %d\n", enrollment_count);
    
    // 学生统计
    printf("\n=== 学生统计 ===\n");
    int active_students = 0;
    int inactive_students = 0;
    float total_gpa = 0.0;
    int total_credits = 0;
    int year_counts[5] = {0}; // FRESHMAN, SOPHOMORE, JUNIOR, SENIOR, GRADUATE
    
    for (int i = 0; i < student_count; i++) {
        if (students[i].is_active) {
            active_students++;
        } else {
            inactive_students++;
        }
        
        total_gpa += students[i].gpa;
        total_credits += students[i].total_credits;
        year_counts[students[i].year]++;
    }
    
    printf("在读学生数: %d\n", active_students);
    printf("休学/毕业学生数: %d\n", inactive_students);
    if (student_count > 0) {
        printf("平均GPA: %.2f\n", total_gpa / student_count);
        printf("平均总学分: %.2f\n", (float)total_credits / student_count);
    }
    printf("年级分布:\n");
    printf("  大一: %d\n", year_counts[FRESHMAN]);
    printf("  大二: %d\n", year_counts[SOPHOMORE]);
    printf("  大三: %d\n", year_counts[JUNIOR]);
    printf("  大四: %d\n", year_counts[SENIOR]);
    printf("  研究生: %d\n", year_counts[GRADUATE]);
    
    // 教师统计
    printf("\n=== 教师统计 ===\n");
    int active_teachers = 0;
    int inactive_teachers = 0;
    float total_salary = 0.0;
    int gender_counts[3] = {0}; // MALE, FEMALE, OTHER
    
    for (int i = 0; i < teacher_count; i++) {
        if (teachers[i].is_active) {
            active_teachers++;
        } else {
            inactive_teachers++;
        }
        
        total_salary += teachers[i].salary;
        gender_counts[teachers[i].gender]++;
    }
    
    printf("在职教师数: %d\n", active_teachers);
    printf("离职教师数: %d\n", inactive_teachers);
    if (teacher_count > 0) {
        printf("平均薪资: %.2f\n", total_salary / teacher_count);
    }
    printf("性别分布:\n");
    printf("  男: %d\n", gender_counts[MALE]);
    printf("  女: %d\n", gender_counts[FEMALE]);
    printf("  其他: %d\n", gender_counts[OTHER]);
    
    // 课程统计
    printf("\n=== 课程统计 ===\n");
    int total_enrollments = 0;
    int semester_counts[4] = {0}; // SPRING, SUMMER, FALL, WINTER
    int credit_counts[10] = {0}; // 统计不同学分的课程数
    
    for (int i = 0; i < course_count; i++) {
        total_enrollments += courses[i].current_students;
        semester_counts[courses[i].semester]++;
        
        if (courses[i].credits >= 1 && courses[i].credits <= 9) {
            credit_counts[courses[i].credits]++;
        } else {
            credit_counts[0]++; // 其他学分
        }
    }
    
    printf("总选课人次: %d\n", total_enrollments);
    printf("学期分布:\n");
    printf("  春季: %d\n", semester_counts[SPRING]);
    printf("  夏季: %d\n", semester_counts[SUMMER]);
    printf("  秋季: %d\n", semester_counts[FALL]);
    printf("  冬季: %d\n", semester_counts[WINTER]);
    printf("学分分布:\n");
    for (int i = 1; i <= 9; i++) {
        if (credit_counts[i] > 0) {
            printf("  %d学分: %d\n", i, credit_counts[i]);
        }
    }
    if (credit_counts[0] > 0) {
        printf("  其他学分: %d\n", credit_counts[0]);
    }
    
    // 成绩统计
    printf("\n=== 成绩统计 ===\n");
    int completed_enrollments = 0;
    float total_score = 0.0;
    int grade_counts[5] = {0}; // A, B, C, D, F
    
    for (int i = 0; i < enrollment_count; i++) {
        if (enrollments[i].completed) {
            completed_enrollments++;
            total_score += enrollments[i].average;
            
            // 统计等级
            if (enrollments[i].average >= 90) grade_counts[0]++;
            else if (enrollments[i].average >= 80) grade_counts[1]++;
            else if (enrollments[i].average >= 70) grade_counts[2]++;
            else if (enrollments[i].average >= 60) grade_counts[3]++;
            else grade_counts[4]++;
        }
    }
    
    printf("完成课程人次: %d\n", completed_enrollments);
    printf("未完成课程人次: %d\n", enrollment_count - completed_enrollments);
    if (completed_enrollments > 0) {
        printf("平均成绩: %.2f\n", total_score / completed_enrollments);
    }
    printf("等级分布:\n");
    printf("  A (90-100): %d\n", grade_counts[0]);
    printf("  B (80-89): %d\n", grade_counts[1]);
    printf("  C (70-79): %d\n", grade_counts[2]);
    printf("  D (60-69): %d\n", grade_counts[3]);
    printf("  F (0-59): %d\n", grade_counts[4]);
    
    // 班级统计
    printf("\n=== 班级统计 ===\n");
    int total_class_students = 0;
    int classes_with_teacher = 0;
    int classes_without_teacher = 0;
    
    for (int i = 0; i < class_count; i++) {
        total_class_students += classes[i].student_count;
        
        if (strlen(classes[i].teacher_id) > 0) {
            classes_with_teacher++;
        } else {
            classes_without_teacher++;
        }
    }
    
    printf("班级学生总数: %d\n", total_class_students);
    printf("有负责教师的班级数: %d\n", classes_with_teacher);
    printf("无负责教师的班级数: %d\n", classes_without_teacher);
    
    // 院系统计
    printf("\n=== 院系统计 ===\n");
    char departments[MAX_DEPARTMENT_LEN][MAX_DEPARTMENT_LEN];
    int dept_count = 0;
    
    // 收集所有院系
    for (int i = 0; i < student_count; i++) {
        bool found = false;
        for (int j = 0; j < dept_count; j++) {
            if (strcmp(students[i].department, departments[j]) == 0) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            strcpy(departments[dept_count], students[i].department);
            dept_count++;
        }
    }
    
    for (int i = 0; i < teacher_count; i++) {
        bool found = false;
        for (int j = 0; j < dept_count; j++) {
            if (strcmp(teachers[i].department, departments[j]) == 0) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            strcpy(departments[dept_count], teachers[i].department);
            dept_count++;
        }
    }
    
    for (int i = 0; i < course_count; i++) {
        bool found = false;
        for (int j = 0; j < dept_count; j++) {
            if (strcmp(courses[i].department, departments[j]) == 0) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            strcpy(departments[dept_count], courses[i].department);
            dept_count++;
        }
    }
    
    printf("院系数: %d\n", dept_count);
    
    // 显示各院系的基本统计
    for (int i = 0; i < dept_count; i++) {
        int dept_students = 0;
        int dept_teachers = 0;
        int dept_courses = 0;
        
        for (int j = 0; j < student_count; j++) {
            if (strcmp(students[j].department, departments[i]) == 0) {
                dept_students++;
            }
        }
        
        for (int j = 0; j < teacher_count; j++) {
            if (strcmp(teachers[j].department, departments[i]) == 0) {
                dept_teachers++;
            }
        }
        
        for (int j = 0; j < course_count; j++) {
            if (strcmp(courses[j].department, departments[i]) == 0) {
                dept_courses++;
            }
        }
        
        printf("%s: 学生 %d, 教师 %d, 课程 %d\n", 
               departments[i], dept_students, dept_teachers, dept_courses);
    }
}

// 备份数据
void backup_data() {
    printf("\n=== 备份数据 ===\n");
    
    char filename[100];
    printf("请输入备份文件名 (默认: backup.dat): ");
    get_string_input("", filename, 100);
    
    if (strlen(filename) == 0) {
        strcpy(filename, "backup.dat");
    }
    
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("无法创建备份文件。\n");
        return;
    }
    
    // 写入学生数据
    fwrite(&student_count, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), student_count, file);
    
    // 写入教师数据
    fwrite(&teacher_count, sizeof(int), 1, file);
    fwrite(teachers, sizeof(Teacher), teacher_count, file);
    
    // 写入课程数据
    fwrite(&course_count, sizeof(int), 1, file);
    fwrite(courses, sizeof(Course), course_count, file);
    
    // 写入班级数据
    fwrite(&class_count, sizeof(int), 1, file);
    fwrite(classes, sizeof(Class), class_count, file);
    
    // 写入选课数据
    fwrite(&enrollment_count, sizeof(int), 1, file);
    fwrite(enrollments, sizeof(Enrollment), enrollment_count, file);
    
    fclose(file);
    
    printf("数据已成功备份到 %s\n", filename);
}

// 恢复数据
void restore_data() {
    printf("\n=== 恢复数据 ===\n");
    
    char filename[100];
    printf("请输入备份文件名 (默认: backup.dat): ");
    get_string_input("", filename, 100);
    
    if (strlen(filename) == 0) {
        strcpy(filename, "backup.dat");
    }
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("无法打开备份文件。\n");
        return;
    }
    
    // 确认恢复
    printf("警告: 恢复数据将覆盖当前所有数据，是否继续? (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    clear_input_buffer();
    
    if (confirm != 'y' && confirm != 'Y') {
        fclose(file);
        printf("恢复已取消。\n");
        return;
    }
    
    // 读取学生数据
    fread(&student_count, sizeof(int), 1, file);
    fread(students, sizeof(Student), student_count, file);
    
    // 读取教师数据
    fread(&teacher_count, sizeof(int), 1, file);
    fread(teachers, sizeof(Teacher), teacher_count, file);
    
    // 读取课程数据
    fread(&course_count, sizeof(int), 1, file);
    fread(courses, sizeof(Course), course_count, file);
    
    // 读取班级数据
    fread(&class_count, sizeof(int), 1, file);
    fread(classes, sizeof(Class), class_count, file);
    
    // 读取选课数据
    fread(&enrollment_count, sizeof(int), 1, file);
    fread(enrollments, sizeof(Enrollment), enrollment_count, file);
    
    fclose(file);
    
    // 重建哈希表
    init_hash_table(&student_hash);
    init_hash_table(&course_hash);
    init_hash_table(&teacher_hash);
    init_hash_table(&class_hash);
    
    for (int i = 0; i < student_count; i++) {
        insert_hash(&student_hash, students[i].id, &students[i]);
    }
    
    for (int i = 0; i < course_count; i++) {
        insert_hash(&course_hash, courses[i].course_id, &courses[i]);
    }
    
    for (int i = 0; i < teacher_count; i++) {
        insert_hash(&teacher_hash, teachers[i].id, &teachers[i]);
    }
    
    for (int i = 0; i < class_count; i++) {
        insert_hash(&class_hash, classes[i].id, &classes[i]);
    }
    
    printf("数据已成功从 %s 恢复\n", filename);
}

// 修改密码
void change_password() {
    printf("\n=== 修改密码 ===\n");
    
    char old_password[MAX_PASSWORD_LEN];
    char new_password[MAX_PASSWORD_LEN];
    char confirm_password[MAX_PASSWORD_LEN];
    
    printf("请输入当前密码: ");
    get_string_input("", old_password, MAX_PASSWORD_LEN);
    
    // 验证当前密码
    bool password_valid = false;
    
    if (current_session.role == ADMIN) {
        if (strcmp(old_password, "admin123") == 0) {
            password_valid = true;
        }
    } else if (current_session.role == TEACHER) {
        for (int i = 0; i < teacher_count; i++) {
            if (strcmp(teachers[i].id, current_session.username) == 0 && 
                strcmp(teachers[i].password, old_password) == 0) {
                password_valid = true;
                break;
            }
        }
    } else if (current_session.role == STUDENT) {
        for (int i = 0; i < student_count; i++) {
            if (strcmp(students[i].id, current_session.username) == 0 && 
                strcmp(students[i].password, old_password) == 0) {
                password_valid = true;
                break;
            }
        }
    }
    
    if (!password_valid) {
        printf("当前密码不正确。\n");
        return;
    }
    
    printf("请输入新密码: ");
    get_string_input("", new_password, MAX_PASSWORD_LEN);
    
    printf("请再次输入新密码: ");
    get_string_input("", confirm_password, MAX_PASSWORD_LEN);
    
    if (strcmp(new_password, confirm_password) != 0) {
        printf("两次输入的密码不一致。\n");
        return;
    }
    
    // 更新密码
    if (current_session.role == ADMIN) {
        // 管理员密码存储在students[0]
        strcpy(students[0].password, new_password);
    } else if (current_session.role == TEACHER) {
        for (int i = 0; i < teacher_count; i++) {
            if (strcmp(teachers[i].id, current_session.username) == 0) {
                strcpy(teachers[i].password, new_password);
                break;
            }
        }
    } else if (current_session.role == STUDENT) {
        for (int i = 0; i < student_count; i++) {
            if (strcmp(students[i].id, current_session.username) == 0) {
                strcpy(students[i].password, new_password);
                break;
            }
        }
    }
    
    printf("密码修改成功！\n");
}

// 系统设置
void system_settings() {
    printf("\n=== 系统设置 ===\n");
    printf("1. 修改最大学生数\n");
    printf("2. 修改最大课程数\n");
    printf("3. 修改最大教师数\n");
    printf("4. 修改最大班级数\n");
    printf("5. 修改最大选课记录数\n");
    printf("6. 重置系统数据\n");
    printf("7. 返回\n");
    printf("请选择操作: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1: {
            printf("当前最大学生数: %d\n", MAX_STUDENTS);
            printf("注意: 修改此值需要重新编译程序。\n");
            break;
        }
        case 2: {
            printf("当前最大课程数: %d\n", MAX_COURSES);
            printf("注意: 修改此值需要重新编译程序。\n");
            break;
        }
        case 3: {
            printf("当前最大教师数: %d\n", MAX_TEACHERS);
            printf("注意: 修改此值需要重新编译程序。\n");
            break;
        }
        case 4: {
            printf("当前最大班级数: %d\n", MAX_CLASSES);
            printf("注意: 修改此值需要重新编译程序。\n");
            break;
        }
        case 5: {
            printf("当前最大选课记录数: %d\n", MAX_ENROLLMENTS);
            printf("注意: 修改此值需要重新编译程序。\n");
            break;
        }
        case 6: {
            printf("警告: 重置系统数据将删除所有数据，是否继续? (y/n): ");
            char confirm;
            scanf(" %c", &confirm);
            clear_input_buffer();
            
            if (confirm == 'y' || confirm == 'Y') {
                // 重置所有数据
                student_count = 0;
                teacher_count = 0;
                course_count = 0;
                class_count = 0;
                enrollment_count = 0;
                
                // 重新初始化系统
                initialize_system();
                
                printf("系统数据已重置。\n");
            } else {
                printf("重置已取消。\n");
            }
            break;
        }
        case 7:
            return;
        default:
            printf("无效选择。\n");
    }
}

// 查看审计日志
void view_audit_log() {
    printf("\n=== 审计日志 ===\n");
    printf("此功能尚未实现。\n");
    printf("在实际系统中，这里会显示用户登录、数据修改等操作的日志记录。\n");
}

// 清除输入缓冲区
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 获取整数输入
int get_int_input(const char *prompt) {
    int value;
    char buffer[100];
    
    while (1) {
        if (strlen(prompt) > 0) {
            printf("%s", prompt);
        }
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("输入错误。\n");
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            return value;
        } else {
            printf("无效输入，请输入一个整数。\n");
        }
    }
}

// 获取浮点数输入
float get_float_input(const char *prompt) {
    float value;
    char buffer[100];
    
    while (1) {
        if (strlen(prompt) > 0) {
            printf("%s", prompt);
        }
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("输入错误。\n");
            continue;
        }
        
        if (sscanf(buffer, "%f", &value) == 1) {
            return value;
        } else {
            printf("无效输入，请输入一个数字。\n");
        }
    }
}

// 获取字符串输入
void get_string_input(const char *prompt, char *buffer, int max_len) {
    if (strlen(prompt) > 0) {
        printf("%s", prompt);
    }
    
    if (fgets(buffer, max_len, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    
    // 去除换行符
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    // 去除前后空格
    trim_whitespace(buffer);
}

// 获取日期输入
Date get_date_input(const char *prompt) {
    Date date;
    char buffer[100];
    
    while (1) {
        if (strlen(prompt) > 0) {
            printf("%s", prompt);
        }
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("输入错误。\n");
            continue;
        }
        
        if (sscanf(buffer, "%d %d %d", &date.day, &date.month, &date.year) == 3) {
            // 简单验证日期
            if (date.year < 1900 || date.year > 2100) {
                printf("年份无效，请重新输入。\n");
                continue;
            }
            
            if (date.month < 1 || date.month > 12) {
                printf("月份无效，请重新输入。\n");
                continue;
            }
            
            int max_day = 31;
            if (date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11) {
                max_day = 30;
            } else if (date.month == 2) {
                // 简单判断闰年
                if ((date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0)) {
                    max_day = 29;
                } else {
                    max_day = 28;
                }
            }
            
            if (date.day < 1 || date.day > max_day) {
                printf("日期无效，请重新输入。\n");
                continue;
            }
            
            return date;
        } else {
            printf("无效输入，请按照格式输入 (日 月 年)。\n");
        }
    }
}

// 获取性别输入
Gender get_gender_input(const char *prompt) {
    int value;
    
    while (1) {
        value = get_int_input(prompt);
        
        if (value >= 0 && value <= 2) {
            return (Gender)value;
        } else {
            printf("无效输入，请输入 0-男, 1-女, 2-其他。\n");
        }
    }
}

// 获取年级输入
Year get_year_input(const char *prompt) {
    int value;
    
    while (1) {
        value = get_int_input(prompt);
        
        if (value >= 0 && value <= 4) {
            return (Year)value;
        } else {
            printf("无效输入，请输入 0-大一, 1-大二, 2-大三, 3-大四, 4-研究生。\n");
        }
    }
}

// 获取成绩等级输入
Grade get_grade_input(const char *prompt) {
    int value;
    
    while (1) {
        value = get_int_input(prompt);
        
        if (value >= 0 && value <= 4) {
            return (Grade)value;
        } else {
            printf("无效输入，请输入 0-A, 1-B, 2-C, 3-D, 4-F。\n");
        }
    }
}

// 获取学期输入
Semester get_semester_input(const char *prompt) {
    int value;
    
    while (1) {
        value = get_int_input(prompt);
        
        if (value >= 0 && value <= 3) {
            return (Semester)value;
        } else {
            printf("无效输入，请输入 0-春季, 1-夏季, 2-秋季, 3-冬季。\n");
        }
    }
}

// 获取用户角色输入
UserRole get_role_input(const char *prompt) {
    int value;
    
    while (1) {
        value = get_int_input(prompt);
        
        if (value >= 0 && value <= 3) {
            return (UserRole)value;
        } else {
            printf("无效输入，请输入 0-管理员, 1-教师, 2-学生, 3-访客。\n");
        }
    }
}

// 验证邮箱格式
bool is_valid_email(const char *email) {
    if (strlen(email) == 0) {
        return false;
    }
    
    int at_count = 0;
    int dot_after_at = 0;
    
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            at_count++;
            if (i == 0 || email[i + 1] == '\0') {
                return false;
            }
        } else if (email[i] == '.' && at_count > 0) {
            if (email[i + 1] == '\0') {
                return false;
            }
            dot_after_at++;
        }
    }
    
    return (at_count == 1 && dot_after_at >= 1);
}

// 验证电话号码格式
bool is_valid_phone(const char *phone) {
    if (strlen(phone) == 0) {
        return false;
    }
    
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i])) {
            return false;
        }
    }
    
    return (strlen(phone) >= 7 && strlen(phone) <= 15);
}

// 验证ID格式
bool is_valid_id(const char *id) {
    if (strlen(id) == 0) {
        return false;
    }
    
    for (int i = 0; id[i] != '\0'; i++) {
        if (!isalnum(id[i])) {
            return false;
        }
    }
    
    return true;
}

// 转换为小写
void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + 32;
        }
    }
}

// 转换为大写
void to_upper_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 32;
        }
    }
}

// 去除字符串前后空格
void trim_whitespace(char *str) {
    int i = 0;
    int j = strlen(str) - 1;
    
    // 去除前导空格
    while (str[i] == ' ' || str[i] == '\t') {
        i++;
    }
    
    // 去除尾部空格
    while (j >= i && (str[j] == ' ' || str[j] == '\t')) {
        j--;
    }
    
    // 移动字符串
    if (i > 0) {
        int k;
        for (k = 0; k <= j - i; k++) {
            str[k] = str[i + k];
        }
        str[k] = '\0';
    } else {
        str[j + 1] = '\0';
    }
}

// 哈希函数
unsigned int hash_function(const char *key) {
    unsigned int hash = 5381;
    int c;
    
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    
    return hash % HASH_TABLE_SIZE;
}

// 初始化哈希表
void init_hash_table(HashTable *ht) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
    ht->size = 0;
}

// 插入哈希表
void insert_hash(HashTable *ht, const char *key, void *data) {
    unsigned int index = hash_function(key);
    
    HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
    if (new_node == NULL) {
        printf("内存分配失败。\n");
        return;
    }
    
    strcpy(new_node->key, key);
    new_node->data = data;
    new_node->next = ht->table[index];
    ht->table[index] = new_node;
    ht->size++;
}

// 查找哈希表
void *lookup_hash(HashTable *ht, const char *key) {
    unsigned int index = hash_function(key);
    HashNode *node = ht->table[index];
    
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            return node->data;
        }
        node = node->next;
    }
    
    return NULL;
}

// 从哈希表中删除
void remove_hash(HashTable *ht, const char *key) {
    unsigned int index = hash_function(key);
    HashNode *node = ht->table[index];
    HashNode *prev = NULL;
    
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            if (prev == NULL) {
                ht->table[index] = node->next;
            } else {
                prev->next = node->next;
            }
            free(node);
            ht->size--;
            return;
        }
        prev = node;
        node = node->next;
    }
}

// 释放哈希表
void free_hash_table(HashTable *ht) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HashNode *node = ht->table[i];
        while (node != NULL) {
            HashNode *temp = node;
            node = node->next;
            free(temp);
        }
        ht->table[i] = NULL;
    }
    ht->size = 0;
}

// 保存数据
void save_data() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("无法保存数据。\n");
        return;
    }
    
    // 写入学生数据
    fwrite(&student_count, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), student_count, file);
    
    // 写入教师数据
    fwrite(&teacher_count, sizeof(int), 1, file);
    fwrite(teachers, sizeof(Teacher), teacher_count, file);
    
    // 写入课程数据
    fwrite(&course_count, sizeof(int), 1, file);
    fwrite(courses, sizeof(Course), course_count, file);
    
    // 写入班级数据
    fwrite(&class_count, sizeof(int), 1, file);
    fwrite(classes, sizeof(Class), class_count, file);
    
    // 写入选课数据
    fwrite(&enrollment_count, sizeof(int), 1, file);
    fwrite(enrollments, sizeof(Enrollment), enrollment_count, file);
    
    fclose(file);
}

// 加载数据
void load_data() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("未找到数据文件，将使用默认数据。\n");
        return;
    }
    
    // 读取学生数据
    fread(&student_count, sizeof(int), 1, file);
    fread(students, sizeof(Student), student_count, file);
    
    // 读取教师数据
    fread(&teacher_count, sizeof(int), 1, file);
    fread(teachers, sizeof(Teacher), teacher_count, file);
    
    // 读取课程数据
    fread(&course_count, sizeof(int), 1, file);
    fread(courses, sizeof(Course), course_count, file);
    
    // 读取班级数据
    fread(&class_count, sizeof(int), 1, file);
    fread(classes, sizeof(Class), class_count, file);
    
    // 读取选课数据
    fread(&enrollment_count, sizeof(int), 1, file);
    fread(enrollments, sizeof(Enrollment), enrollment_count, file);
    
    fclose(file);
    
    // 重建哈希表
    init_hash_table(&student_hash);
    init_hash_table(&course_hash);
    init_hash_table(&teacher_hash);
    init_hash_table(&class_hash);
    
    for (int i = 0; i < student_count; i++) {
        insert_hash(&student_hash, students[i].id, &students[i]);
    }
    
    for (int i = 0; i < course_count; i++) {
        insert_hash(&course_hash, courses[i].course_id, &courses[i]);
    }
    
    for (int i = 0; i < teacher_count; i++) {
        insert_hash(&teacher_hash, teachers[i].id, &teachers[i]);
    }
    
    for (int i = 0; i < class_count; i++) {
        insert_hash(&class_hash, classes[i].id, &classes[i]);
    }
}

// 写入字符串到文件
void write_string(FILE *file, const char *str) {
    int len = strlen(str);
    fwrite(&len, sizeof(int), 1, file);
    fwrite(str, sizeof(char), len, file);
}

// 从文件读取字符串
void read_string(FILE *file, char *buffer, int max_len) {
    int len;
    fread(&len, sizeof(int), 1, file);
    
    if (len >= max_len) {
        len = max_len - 1;
    }
    
    fread(buffer, sizeof(char), len, file);
    buffer[len] = '\0';
    
    // 跳过剩余字符
    if (len < max_len - 1) {
        fseek(file, strlen(buffer) - len, SEEK_CUR);
    }
}

// 比较学生姓名
int compare_students_by_name(const void *a, const void *b) {
    const Student *student_a = (const Student *)a;
    const Student *student_b = (const Student *)b;
    return strcmp(student_a->name, student_b->name);
}

// 比较学生ID
int compare_students_by_id(const void *a, const void *b) {
    const Student *student_a = (const Student *)a;
    const Student *student_b = (const Student *)b;
    return strcmp(student_a->id, student_b->id);
}

// 比较学生GPA
int compare_students_by_gpa(const void *a, const void *b) {
    const Student *student_a = (const Student *)a;
    const Student *student_b = (const Student *)b;
    if (student_a->gpa < student_b->gpa) return 1;
    if (student_a->gpa > student_b->gpa) return -1;
    return 0;
}

// 比较课程名称
int compare_courses_by_name(const void *a, const void *b) {
    const Course *course_a = (const Course *)a;
    const Course *course_b = (const Course *)b;
    return strcmp(course_a->name, course_b->name);
}

// 比较课程ID
int compare_courses_by_id(const void *a, const void *b) {
    const Course *course_a = (const Course *)a;
    const Course *course_b = (const Course *)b;
    return strcmp(course_a->course_id, course_b->course_id);
}

// 比较课程学分
int compare_courses_by_credits(const void *a, const void *b) {
    const Course *course_a = (const Course *)a;
    const Course *course_b = (const Course *)b;
    if (course_a->credits < course_b->credits) return 1;
    if (course_a->credits > course_b->credits) return -1;
    return 0;
}

// 比较教师姓名
int compare_teachers_by_name(const void *a, const void *b) {
    const Teacher *teacher_a = (const Teacher *)a;
    const Teacher *teacher_b = (const Teacher *)b;
    return strcmp(teacher_a->name, teacher_b->name);
}

// 比较教师ID
int compare_teachers_by_id(const void *a, const void *b) {
    const Teacher *teacher_a = (const Teacher *)a;
    const Teacher *teacher_b = (const Teacher *)b;
    return strcmp(teacher_a->id, teacher_b->id);
}

// 比较教师薪资
int compare_teachers_by_salary(const void *a, const void *b) {
    const Teacher *teacher_a = (const Teacher *)a;
    const Teacher *teacher_b = (const Teacher *)b;
    if (teacher_a->salary < teacher_b->salary) return 1;
    if (teacher_a->salary > teacher_b->salary) return -1;
    return 0;
}
