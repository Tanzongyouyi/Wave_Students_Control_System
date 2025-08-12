#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

// ��������
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

// ö�����Ͷ���
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

// �ṹ�嶨��
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

// ȫ�ֱ���
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

// ��������
void initialize_system();
void main_menu();
void login();
void logout();
void admin_menu();
void teacher_menu();
void student_menu();
void guest_menu();

// ѧ��������
void add_student();
void edit_student();
void delete_student();
void view_student();
void list_all_students();
void search_student();
void sort_students();

// �γ̹�����
void add_course();
void edit_course();
void delete_course();
void view_course();
void list_all_courses();
void search_course();
void sort_courses();

// ѡ�ι�����
void enroll_student();
void drop_course();
void view_enrollments();
void assign_grade();
void calculate_gpa();

// ��ʦ������
void add_teacher();
void edit_teacher();
void delete_teacher();
void view_teacher();
void list_all_teachers();
void search_teacher();
void sort_teachers();

// �༶������
void add_class();
void edit_class();
void delete_class();
void view_class();
void list_all_classes();
void assign_teacher_to_class();
void assign_student_to_class();

// ͳ�ƺͱ��溯��
void generate_student_report();
void generate_course_report();
void generate_teacher_report();
void generate_department_report();
void generate_statistics();

// ϵͳ������
void backup_data();
void restore_data();
void change_password();
void system_settings();
void view_audit_log();

// ��������
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

// ��ϣ����
unsigned int hash_function(const char *key);
void init_hash_table(HashTable *ht);
void insert_hash(HashTable *ht, const char *key, void *data);
void *lookup_hash(HashTable *ht, const char *key);
void remove_hash(HashTable *ht, const char *key);
void free_hash_table(HashTable *ht);

// �ļ���������
void save_data();
void load_data();
void write_string(FILE *file, const char *str);
void read_string(FILE *file, char *buffer, int max_len);

// ������
int compare_students_by_name(const void *a, const void *b);
int compare_students_by_id(const void *a, const void *b);
int compare_students_by_gpa(const void *a, const void *b);
int compare_courses_by_name(const void *a, const void *b);
int compare_courses_by_id(const void *a, const void *b);
int compare_courses_by_credits(const void *a, const void *b);
int compare_teachers_by_name(const void *a, const void *b);
int compare_teachers_by_id(const void *a, const void *b);
int compare_teachers_by_salary(const void *a, const void *b);

// ������
int main() {
    initialize_system();
    load_data();
    
    printf("\n=== �������ӵ�ѧ������ϵͳ ===\n");
    printf("ϵͳ��ʼ����ɣ������Ѽ��ء�\n");
    
    main_menu();
    
    save_data();
    printf("\nϵͳ�����ѱ��棬��лʹ�ã�\n");
    return 0;
}

// ��ʼ��ϵͳ
void initialize_system() {
    // ��ʼ����ϣ��
    init_hash_table(&student_hash);
    init_hash_table(&course_hash);
    init_hash_table(&teacher_hash);
    init_hash_table(&class_hash);
    
    // ��ʼ���Ự
    memset(&current_session, 0, sizeof(UserSession));
    current_session.role = GUEST;
    current_session.is_logged_in = false;
    
    // �����������
    srand(time(NULL));
    
    // ���һЩĬ������
    if (student_count == 0) {
        // ���Ĭ�Ϲ���Ա
        strcpy(students[0].id, "admin");
        strcpy(students[0].name, "ϵͳ����Ա");
        strcpy(students[0].password, "admin123");
        students[0].gender = OTHER;
        students[0].birth_date = (Date){1, 1, 1980};
        strcpy(students[0].address, "ϵͳ��������");
        strcpy(students[0].phone, "00000000000");
        strcpy(students[0].email, "admin@system.com");
        students[0].year = GRADUATE;
        strcpy(students[0].department, "ϵͳ����");
        students[0].gpa = 4.0;
        students[0].total_credits = 0;
        students[0].admission_date = (Date){1, 1, 2000};
        students[0].is_active = true;
        student_count = 1;
        
        // ���Ĭ�Ͻ�ʦ
        strcpy(teachers[0].id, "teacher1");
        strcpy(teachers[0].name, "�Ž���");
        strcpy(teachers[0].password, "teacher123");
        teachers[0].gender = MALE;
        teachers[0].birth_date = (Date){15, 5, 1975};
        strcpy(teachers[0].department, "�������ѧ");
        strcpy(teachers[0].phone, "13800138000");
        strcpy(teachers[0].email, "zhang@university.edu");
        teachers[0].hire_date = (Date){1, 9, 2000};
        strcpy(teachers[0].title, "����");
        teachers[0].salary = 15000.0;
        teachers[0].is_active = true;
        teacher_count = 1;
        
        // ���Ĭ�Ͽγ�
        strcpy(courses[0].course_id, "CS101");
        strcpy(courses[0].name, "�������ѧ����");
        courses[0].credits = 3;
        strcpy(courses[0].department, "�������ѧ");
        strcpy(courses[0].teacher_id, "teacher1");
        courses[0].semester = FALL;
        courses[0].year = 2023;
        courses[0].max_students = 100;
        courses[0].current_students = 0;
        course_count = 1;
        
        // ���Ĭ�ϰ༶
        strcpy(classes[0].id, "CS101A");
        strcpy(classes[0].name, "�������ѧ1��");
        strcpy(classes[0].department, "�������ѧ");
        strcpy(classes[0].teacher_id, "teacher1");
        classes[0].student_count = 0;
        classes[0].max_students = 50;
        class_count = 1;
    }
}

// ���˵�
void main_menu() {
    int choice;
    
    while (1) {
        printf("\n=== ���˵� ===\n");
        printf("1. ��¼ϵͳ\n");
        printf("2. �ÿ�ģʽ\n");
        printf("3. �˳�ϵͳ\n");
        printf("��ѡ�����: ");
        
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
                printf("��Чѡ�����������롣\n");
        }
    }
}

// ��¼����
void login() {
    char username[MAX_ID_LEN];
    char password[MAX_PASSWORD_LEN];
    int attempts = 3;
    
    while (attempts > 0) {
        printf("\n=== ϵͳ��¼ ===\n");
        printf("�û���: ");
        get_string_input("", username, MAX_ID_LEN);
        printf("����: ");
        get_string_input("", password, MAX_PASSWORD_LEN);
        
        // ������Ա
        if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
            strcpy(current_session.username, username);
            strcpy(current_session.password, password);
            current_session.role = ADMIN;
            current_session.is_logged_in = true;
            current_session.last_login = time(NULL);
            current_session.session_timeout = time(NULL) + 1800; // 30���ӳ�ʱ
            printf("��¼�ɹ�����ӭ����Ա��\n");
            return;
        }
        
        // ����ʦ
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
                printf("��¼�ɹ�����ӭ %s ��ʦ��\n", teachers[i].name);
                return;
            }
        }
        
        // ���ѧ��
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
                printf("��¼�ɹ�����ӭ %s ͬѧ��\n", students[i].name);
                return;
            }
        }
        
        attempts--;
        printf("�û�����������󣬻�ʣ %d �γ��Ի��ᡣ\n", attempts);
    }
    
    printf("��¼ʧ�ܣ����Ժ����ԡ�\n");
}

// �ǳ�����
void logout() {
    if (current_session.is_logged_in) {
        printf("�û� %s �ѵǳ���\n", current_session.username);
        memset(&current_session, 0, sizeof(UserSession));
        current_session.role = GUEST;
        current_session.is_logged_in = false;
    }
}

// ����Ա�˵�
void admin_menu() {
    int choice;
    
    while (1) {
        // ���Ự�Ƿ�ʱ
        if (time(NULL) > current_session.session_timeout) {
            printf("�Ự�ѳ�ʱ�������µ�¼��\n");
            logout();
            return;
        }
        
        printf("\n=== ����Ա�˵� ===\n");
        printf("1. ѧ������\n");
        printf("2. �γ̹���\n");
        printf("3. ��ʦ����\n");
        printf("4. �༶����\n");
        printf("5. ѡ�ι���\n");
        printf("6. ͳ�Ʊ���\n");
        printf("7. ϵͳ����\n");
        printf("8. �޸�����\n");
        printf("9. �ǳ�\n");
        printf("��ѡ�����: ");
        
        choice = get_int_input("");
        
        switch (choice) {
            case 1: {
                int sub_choice;
                printf("\n=== ѧ������ ===\n");
                printf("1. ���ѧ��\n");
                printf("2. �༭ѧ��\n");
                printf("3. ɾ��ѧ��\n");
                printf("4. �鿴ѧ��\n");
                printf("5. �г�����ѧ��\n");
                printf("6. ����ѧ��\n");
                printf("7. ����ѧ��\n");
                printf("8. ����\n");
                printf("��ѡ�����: ");
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
                    default: printf("��Чѡ��\n");
                }
                break;
            }
            case 2: {
                int sub_choice;
                printf("\n=== �γ̹��� ===\n");
                printf("1. ��ӿγ�\n");
                printf("2. �༭�γ�\n");
                printf("3. ɾ���γ�\n");
                printf("4. �鿴�γ�\n");
                printf("5. �г����пγ�\n");
                printf("6. �����γ�\n");
                printf("7. ����γ�\n");
                printf("8. ����\n");
                printf("��ѡ�����: ");
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
                    default: printf("��Чѡ��\n");
                }
                break;
            }
            case 3: {
                int sub_choice;
                printf("\n=== ��ʦ���� ===\n");
                printf("1. ��ӽ�ʦ\n");
                printf("2. �༭��ʦ\n");
                printf("3. ɾ����ʦ\n");
                printf("4. �鿴��ʦ\n");
                printf("5. �г����н�ʦ\n");
                printf("6. ������ʦ\n");
                printf("7. �����ʦ\n");
                printf("8. ����\n");
                printf("��ѡ�����: ");
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
                    default: printf("��Чѡ��\n");
                }
                break;
            }
            case 4: {
                int sub_choice;
                printf("\n=== �༶���� ===\n");
                printf("1. ��Ӱ༶\n");
                printf("2. �༭�༶\n");
                printf("3. ɾ���༶\n");
                printf("4. �鿴�༶\n");
                printf("5. �г����а༶\n");
                printf("6. �����ʦ���༶\n");
                printf("7. ����ѧ�����༶\n");
                printf("8. ����\n");
                printf("��ѡ�����: ");
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
                    default: printf("��Чѡ��\n");
                }
                break;
            }
            case 5: {
                int sub_choice;
                printf("\n=== ѡ�ι��� ===\n");
                printf("1. ѧ��ѡ��\n");
                printf("2. ѧ���˿�\n");
                printf("3. �鿴ѡ�μ�¼\n");
                printf("4. ����ɼ�\n");
                printf("5. ����GPA\n");
                printf("6. ����\n");
                printf("��ѡ�����: ");
                sub_choice = get_int_input("");
                
                switch (sub_choice) {
                    case 1: enroll_student(); break;
                    case 2: drop_course(); break;
                    case 3: view_enrollments(); break;
                    case 4: assign_grade(); break;
                    case 5: calculate_gpa(); break;
                    case 6: break;
                    default: printf("��Чѡ��\n");
                }
                break;
            }
            case 6: {
                int sub_choice;
                printf("\n=== ͳ�Ʊ��� ===\n");
                printf("1. ����ѧ������\n");
                printf("2. ���ɿγ̱���\n");
                printf("3. ���ɽ�ʦ����\n");
                printf("4. ����Ժϵ����\n");
                printf("5. ����ͳ����Ϣ\n");
                printf("6. ����\n");
                printf("��ѡ�����: ");
                sub_choice = get_int_input("");
                
                switch (sub_choice) {
                    case 1: generate_student_report(); break;
                    case 2: generate_course_report(); break;
                    case 3: generate_teacher_report(); break;
                    case 4: generate_department_report(); break;
                    case 5: generate_statistics(); break;
                    case 6: break;
                    default: printf("��Чѡ��\n");
                }
                break;
            }
            case 7: {
                int sub_choice;
                printf("\n=== ϵͳ���� ===\n");
                printf("1. ��������\n");
                printf("2. �ָ�����\n");
                printf("3. ϵͳ����\n");
                printf("4. �鿴�����־\n");
                printf("5. ����\n");
                printf("��ѡ�����: ");
                sub_choice = get_int_input("");
                
                switch (sub_choice) {
                    case 1: backup_data(); break;
                    case 2: restore_data(); break;
                    case 3: system_settings(); break;
                    case 4: view_audit_log(); break;
                    case 5: break;
                    default: printf("��Чѡ��\n");
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
                printf("��Чѡ�����������롣\n");
        }
    }
}

// ��ʦ�˵�
void teacher_menu() {
    int choice;
    
    while (1) {
        // ���Ự�Ƿ�ʱ
        if (time(NULL) > current_session.session_timeout) {
            printf("�Ự�ѳ�ʱ�������µ�¼��\n");
            logout();
            return;
        }
        
        printf("\n=== ��ʦ�˵� ===\n");
        printf("1. �鿴������Ϣ\n");
        printf("2. �޸ĸ�����Ϣ\n");
        printf("3. �鿴���̿γ�\n");
        printf("4. �鿴�γ�ѧ��\n");
        printf("5. ����ɼ�\n");
        printf("6. ���ɿγ̱���\n");
        printf("7. �޸�����\n");
        printf("8. �ǳ�\n");
        printf("��ѡ�����: ");
        
        choice = get_int_input("");
        
        switch (choice) {
            case 1: {
                // �鿴��ʦ������Ϣ
                Teacher *teacher = NULL;
                for (int i = 0; i < teacher_count; i++) {
                    if (strcmp(teachers[i].id, current_session.username) == 0) {
                        teacher = &teachers[i];
                        break;
                    }
                }
                
                if (teacher) {
                    printf("\n=== ��ʦ��Ϣ ===\n");
                    printf("ID: %s\n", teacher->id);
                    printf("����: %s\n", teacher->name);
                    printf("�Ա�: %s\n", teacher->gender == MALE ? "��" : 
                           teacher->gender == FEMALE ? "Ů" : "����");
                    printf("��������: %d-%d-%d\n", teacher->birth_date.year, 
                           teacher->birth_date.month, teacher->birth_date.day);
                    printf("Ժϵ: %s\n", teacher->department);
                    printf("�绰: %s\n", teacher->phone);
                    printf("����: %s\n", teacher->email);
                    printf("Ƹ������: %d-%d-%d\n", teacher->hire_date.year, 
                           teacher->hire_date.month, teacher->hire_date.day);
                    printf("ְ��: %s\n", teacher->title);
                    printf("н��: %.2f\n", teacher->salary);
                    printf("״̬: %s\n", teacher->is_active ? "��ְ" : "��ְ");
                } else {
                    printf("δ�ҵ���ʦ��Ϣ��\n");
                }
                break;
            }
            case 2: {
                // �޸Ľ�ʦ������Ϣ
                Teacher *teacher = NULL;
                for (int i = 0; i < teacher_count; i++) {
                    if (strcmp(teachers[i].id, current_session.username) == 0) {
                        teacher = &teachers[i];
                        break;
                    }
                }
                
                if (teacher) {
                    printf("\n=== �޸Ľ�ʦ��Ϣ ===\n");
                    printf("��ǰ����: %s\n", teacher->name);
                    printf("������ (���ձ��ֲ���): ");
                    char new_name[MAX_NAME_LEN];
                    get_string_input("", new_name, MAX_NAME_LEN);
                    if (strlen(new_name) > 0) {
                        strcpy(teacher->name, new_name);
                    }
                    
                    printf("��ǰ�Ա�: %s\n", teacher->gender == MALE ? "��" : 
                           teacher->gender == FEMALE ? "Ů" : "����");
                    printf("���Ա� (0-��, 1-Ů, 2-����, ���ձ��ֲ���): ");
                    char gender_input[10];
                    get_string_input("", gender_input, 10);
                    if (strlen(gender_input) > 0) {
                        int gender_val = atoi(gender_input);
                        if (gender_val >= 0 && gender_val <= 2) {
                            teacher->gender = (Gender)gender_val;
                        }
                    }
                    
                    printf("��ǰ�绰: %s\n", teacher->phone);
                    printf("�µ绰 (���ձ��ֲ���): ");
                    char new_phone[MAX_PHONE_LEN];
                    get_string_input("", new_phone, MAX_PHONE_LEN);
                    if (strlen(new_phone) > 0) {
                        if (is_valid_phone(new_phone)) {
                            strcpy(teacher->phone, new_phone);
                        } else {
                            printf("��Ч�ĵ绰���롣\n");
                        }
                    }
                    
                    printf("��ǰ����: %s\n", teacher->email);
                    printf("������ (���ձ��ֲ���): ");
                    char new_email[MAX_EMAIL_LEN];
                    get_string_input("", new_email, MAX_EMAIL_LEN);
                    if (strlen(new_email) > 0) {
                        if (is_valid_email(new_email)) {
                            strcpy(teacher->email, new_email);
                        } else {
                            printf("��Ч�������ַ��\n");
                        }
                    }
                    
                    printf("��Ϣ�Ѹ��¡�\n");
                } else {
                    printf("δ�ҵ���ʦ��Ϣ��\n");
                }
                break;
            }
            case 3: {
                // �鿴���̿γ�
                printf("\n=== ���̿γ� ===\n");
                bool found = false;
                
                for (int i = 0; i < course_count; i++) {
                    if (strcmp(courses[i].teacher_id, current_session.username) == 0) {
                        printf("�γ�ID: %s\n", courses[i].course_id);
                        printf("�γ�����: %s\n", courses[i].name);
                        printf("ѧ��: %d\n", courses[i].credits);
                        printf("Ժϵ: %s\n", courses[i].department);
                        printf("ѧ��: %s\n", courses[i].semester == SPRING ? "����" : 
                               courses[i].semester == SUMMER ? "�ļ�" : 
                               courses[i].semester == FALL ? "�＾" : "����");
                        printf("���: %d\n", courses[i].year);
                        printf("���ѧ����: %d\n", courses[i].max_students);
                        printf("��ǰѧ����: %d\n", courses[i].current_students);
                        printf("------------------------\n");
                        found = true;
                    }
                }
                
                if (!found) {
                    printf("��Ŀǰû�н����κογ̡�\n");
                }
                break;
            }
            case 4: {
                // �鿴�γ�ѧ��
                printf("\n=== �鿴�γ�ѧ�� ===\n");
                printf("������γ�ID: ");
                char course_id[MAX_ID_LEN];
                get_string_input("", course_id, MAX_ID_LEN);
                
                bool course_found = false;
                bool is_teacher = false;
                
                // ���γ��Ƿ���������ڸý�ʦ
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
                    printf("�γ̲����ڡ�\n");
                } else if (!is_teacher) {
                    printf("��û��Ȩ�޲鿴�˿γ̵�ѧ����\n");
                } else {
                    printf("\n=== �γ� %s ��ѧ���б� ===\n", course_id);
                    bool student_found = false;
                    
                    for (int i = 0; i < enrollment_count; i++) {
                        if (strcmp(enrollments[i].course_id, course_id) == 0) {
                            // ����ѧ����Ϣ
                            for (int j = 0; j < student_count; j++) {
                                if (strcmp(students[j].id, enrollments[i].student_id) == 0) {
                                    printf("ѧ��ID: %s\n", students[j].id);
                                    printf("����: %s\n", students[j].name);
                                    printf("Ժϵ: %s\n", students[j].department);
                                    printf("�꼶: %s\n", students[j].year == FRESHMAN ? "��һ" : 
                                           students[j].year == SOPHOMORE ? "���" : 
                                           students[j].year == JUNIOR ? "����" : 
                                           students[j].year == SENIOR ? "����" : "�о���");
                                    printf("ƽ���ɼ�: %.2f\n", enrollments[i].average);
                                    printf("���״̬: %s\n", enrollments[i].completed ? "�����" : "������");
                                    printf("------------------------\n");
                                    student_found = true;
                                    break;
                                }
                            }
                        }
                    }
                    
                    if (!student_found) {
                        printf("�˿γ�Ŀǰû��ѧ����\n");
                    }
                }
                break;
            }
            case 5: {
                // ����ɼ�
                assign_grade();
                break;
            }
            case 6: {
                // ���ɿγ̱���
                printf("\n=== ���ɿγ̱��� ===\n");
                printf("������γ�ID: ");
                char course_id[MAX_ID_LEN];
                get_string_input("", course_id, MAX_ID_LEN);
                
                bool course_found = false;
                bool is_teacher = false;
                int course_index = -1;
                
                // ���γ��Ƿ���������ڸý�ʦ
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
                    printf("�γ̲����ڡ�\n");
                } else if (!is_teacher) {
                    printf("��û��Ȩ�����ɴ˿γ̵ı��档\n");
                } else {
                    printf("\n=== �γ̱���: %s ===\n", courses[course_index].name);
                    printf("�γ�ID: %s\n", courses[course_index].course_id);
                    printf("ѧ��: %d\n", courses[course_index].credits);
                    printf("Ժϵ: %s\n", courses[course_index].department);
                    printf("ѧ��: %s\n", courses[course_index].semester == SPRING ? "����" : 
                           courses[course_index].semester == SUMMER ? "�ļ�" : 
                           courses[course_index].semester == FALL ? "�＾" : "����");
                    printf("���: %d\n", courses[course_index].year);
                    printf("���ѧ����: %d\n", courses[course_index].max_students);
                    printf("��ǰѧ����: %d\n", courses[course_index].current_students);
                    
                    // ͳ��ѧ���ɼ�
                    int total_students = 0;
                    float total_score = 0.0;
                    int grade_counts[5] = {0}; // A, B, C, D, F
                    
                    for (int i = 0; i < enrollment_count; i++) {
                        if (strcmp(enrollments[i].course_id, course_id) == 0) {
                            total_students++;
                            total_score += enrollments[i].average;
                            
                            // ͳ�Ƶȼ�
                            if (enrollments[i].average >= 90) grade_counts[0]++;
                            else if (enrollments[i].average >= 80) grade_counts[1]++;
                            else if (enrollments[i].average >= 70) grade_counts[2]++;
                            else if (enrollments[i].average >= 60) grade_counts[3]++;
                            else grade_counts[4]++;
                        }
                    }
                    
                    if (total_students > 0) {
                        printf("\n=== �ɼ�ͳ�� ===\n");
                        printf("ѧ������: %d\n", total_students);
                        printf("ƽ����: %.2f\n", total_score / total_students);
                        printf("�ȼ��ֲ�:\n");
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
                        printf("\n�˿γ�Ŀǰû��ѧ���ɼ����ݡ�\n");
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
                printf("��Чѡ�����������롣\n");
        }
    }
}

// ѧ���˵�
void student_menu() {
    int choice;
    
    while (1) {
        // ���Ự�Ƿ�ʱ
        if (time(NULL) > current_session.session_timeout) {
            printf("�Ự�ѳ�ʱ�������µ�¼��\n");
            logout();
            return;
        }
        
        printf("\n=== ѧ���˵� ===\n");
        printf("1. �鿴������Ϣ\n");
        printf("2. �޸ĸ�����Ϣ\n");
        printf("3. �鿴��ѡ�γ�\n");
        printf("4. ѡ��\n");
        printf("5. �˿�\n");
        printf("6. �鿴�ɼ�\n");
        printf("7. �鿴GPA\n");
        printf("8. �޸�����\n");
        printf("9. �ǳ�\n");
        printf("��ѡ�����: ");
        
        choice = get_int_input("");
        
        switch (choice) {
            case 1: {
                // �鿴ѧ��������Ϣ
                Student *student = NULL;
                for (int i = 0; i < student_count; i++) {
                    if (strcmp(students[i].id, current_session.username) == 0) {
                        student = &students[i];
                        break;
                    }
                }
                
                if (student) {
                    printf("\n=== ѧ����Ϣ ===\n");
                    printf("ѧ��: %s\n", student->id);
                    printf("����: %s\n", student->name);
                    printf("�Ա�: %s\n", student->gender == MALE ? "��" : 
                           student->gender == FEMALE ? "Ů" : "����");
                    printf("��������: %d-%d-%d\n", student->birth_date.year, 
                           student->birth_date.month, student->birth_date.day);
                    printf("��ַ: %s\n", student->address);
                    printf("�绰: %s\n", student->phone);
                    printf("����: %s\n", student->email);
                    printf("�꼶: %s\n", student->year == FRESHMAN ? "��һ" : 
                           student->year == SOPHOMORE ? "���" : 
                           student->year == JUNIOR ? "����" : 
                           student->year == SENIOR ? "����" : "�о���");
                    printf("Ժϵ: %s\n", student->department);
                    printf("GPA: %.2f\n", student->gpa);
                    printf("��ѧ��: %d\n", student->total_credits);
                    printf("��ѧ����: %d-%d-%d\n", student->admission_date.year, 
                           student->admission_date.month, student->admission_date.day);
                    printf("״̬: %s\n", student->is_active ? "�ڶ�" : "��ѧ/��ҵ");
                    printf("�༶: %s\n", student->class_id);
                } else {
                    printf("δ�ҵ�ѧ����Ϣ��\n");
                }
                break;
            }
            case 2: {
                // �޸�ѧ��������Ϣ
                Student *student = NULL;
                for (int i = 0; i < student_count; i++) {
                    if (strcmp(students[i].id, current_session.username) == 0) {
                        student = &students[i];
                        break;
                    }
                }
                
                if (student) {
                    printf("\n=== �޸�ѧ����Ϣ ===\n");
                    printf("��ǰ����: %s\n", student->name);
                    printf("������ (���ձ��ֲ���): ");
                    char new_name[MAX_NAME_LEN];
                    get_string_input("", new_name, MAX_NAME_LEN);
                    if (strlen(new_name) > 0) {
                        strcpy(student->name, new_name);
                    }
                    
                    printf("��ǰ��ַ: %s\n", student->address);
                    printf("�µ�ַ (���ձ��ֲ���): ");
                    char new_address[MAX_ADDRESS_LEN];
                    get_string_input("", new_address, MAX_ADDRESS_LEN);
                    if (strlen(new_address) > 0) {
                        strcpy(student->address, new_address);
                    }
                    
                    printf("��ǰ�绰: %s\n", student->phone);
                    printf("�µ绰 (���ձ��ֲ���): ");
                    char new_phone[MAX_PHONE_LEN];
                    get_string_input("", new_phone, MAX_PHONE_LEN);
                    if (strlen(new_phone) > 0) {
                        if (is_valid_phone(new_phone)) {
                            strcpy(student->phone, new_phone);
                        } else {
                            printf("��Ч�ĵ绰���롣\n");
                        }
                    }
                    
                    printf("��ǰ����: %s\n", student->email);
                    printf("������ (���ձ��ֲ���): ");
                    char new_email[MAX_EMAIL_LEN];
                    get_string_input("", new_email, MAX_EMAIL_LEN);
                    if (strlen(new_email) > 0) {
                        if (is_valid_email(new_email)) {
                            strcpy(student->email, new_email);
                        } else {
                            printf("��Ч�������ַ��\n");
                        }
                    }
                    
                    printf("��Ϣ�Ѹ��¡�\n");
                } else {
                    printf("δ�ҵ�ѧ����Ϣ��\n");
                }
                break;
            }
            case 3: {
                // �鿴��ѡ�γ�
                printf("\n=== ��ѡ�γ� ===\n");
                bool found = false;
                
                for (int i = 0; i < enrollment_count; i++) {
                    if (strcmp(enrollments[i].student_id, current_session.username) == 0) {
                        // ���ҿγ���Ϣ
                        for (int j = 0; j < course_count; j++) {
                            if (strcmp(courses[j].course_id, enrollments[i].course_id) == 0) {
                                printf("�γ�ID: %s\n", courses[j].course_id);
                                printf("�γ�����: %s\n", courses[j].name);
                                printf("ѧ��: %d\n", courses[j].credits);
                                printf("Ժϵ: %s\n", courses[j].department);
                                printf("ѧ��: %s\n", courses[j].semester == SPRING ? "����" : 
                                       courses[j].semester == SUMMER ? "�ļ�" : 
                                       courses[j].semester == FALL ? "�＾" : "����");
                                printf("���: %d\n", courses[j].year);
                                printf("ƽ���ɼ�: %.2f\n", enrollments[i].average);
                                printf("���״̬: %s\n", enrollments[i].completed ? "�����" : "������");
                                printf("------------------------\n");
                                found = true;
                                break;
                            }
                        }
                    }
                }
                
                if (!found) {
                    printf("��Ŀǰû��ѡ���κογ̡�\n");
                }
                break;
            }
            case 4: {
                // ѡ��
                printf("\n=== ��ѡ�γ� ===\n");
                bool available = false;
                
                // ��ʾ��ѡ�γ�
                for (int i = 0; i < course_count; i++) {
                    // ���γ��Ƿ�����
                    if (courses[i].current_students >= courses[i].max_students) {
                        continue;
                    }
                    
                    // ���ѧ���Ƿ��Ѿ�ѡ�����ſ�
                    bool already_enrolled = false;
                    for (int j = 0; j < enrollment_count; j++) {
                        if (strcmp(enrollments[j].student_id, current_session.username) == 0 && 
                            strcmp(enrollments[j].course_id, courses[i].course_id) == 0) {
                            already_enrolled = true;
                            break;
                        }
                    }
                    
                    if (!already_enrolled) {
                        printf("�γ�ID: %s\n", courses[i].course_id);
                        printf("�γ�����: %s\n", courses[i].name);
                        printf("ѧ��: %d\n", courses[i].credits);
                        printf("Ժϵ: %s\n", courses[i].department);
                        printf("��ʦID: %s\n", courses[i].teacher_id);
                        printf("ѧ��: %s\n", courses[i].semester == SPRING ? "����" : 
                               courses[i].semester == SUMMER ? "�ļ�" : 
                               courses[i].semester == FALL ? "�＾" : "����");
                        printf("���: %d\n", courses[i].year);
                        printf("��ǰѧ����/���ѧ����: %d/%d\n", 
                               courses[i].current_students, courses[i].max_students);
                        printf("------------------------\n");
                        available = true;
                    }
                }
                
                if (!available) {
                    printf("Ŀǰû�п�ѡ�γ̡�\n");
                } else {
                    printf("������Ҫѡ�޵Ŀγ�ID: ");
                    char course_id[MAX_ID_LEN];
                    get_string_input("", course_id, MAX_ID_LEN);
                    
                    bool course_found = false;
                    bool can_enroll = false;
                    int course_index = -1;
                    
                    // ���γ��Ƿ�����ҿ�ѡ
                    for (int i = 0; i < course_count; i++) {
                        if (strcmp(courses[i].course_id, course_id) == 0) {
                            course_found = true;
                            course_index = i;
                            
                            // ���γ��Ƿ�����
                            if (courses[i].current_students >= courses[i].max_students) {
                                printf("�ÿγ�����Ա��\n");
                            } else {
                                // ���ѧ���Ƿ��Ѿ�ѡ�����ſ�
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
                                    printf("���Ѿ�ѡ�������ſγ̡�\n");
                                }
                            }
                            break;
                        }
                    }
                    
                    if (!course_found) {
                        printf("�γ̲����ڡ�\n");
                    } else if (can_enroll) {
                        // ���ѡ�μ�¼
                        if (enrollment_count < MAX_ENROLLMENTS) {
                            strcpy(enrollments[enrollment_count].student_id, current_session.username);
                            strcpy(enrollments[enrollment_count].course_id, course_id);
                            
                            // ��ʼ���ɼ�
                            for (int j = 0; j < MAX_GRADES; j++) {
                                enrollments[enrollment_count].grades[j] = F;
                            }
                            enrollments[enrollment_count].average = 0.0;
                            
                            // ����ѡ������Ϊ��ǰ����
                            time_t t = time(NULL);
                            struct tm *tm = localtime(&t);
                            enrollments[enrollment_count].enrollment_date.day = tm->tm_mday;
                            enrollments[enrollment_count].enrollment_date.month = tm->tm_mon + 1;
                            enrollments[enrollment_count].enrollment_date.year = tm->tm_year + 1900;
                            
                            enrollments[enrollment_count].completed = false;
                            
                            enrollment_count++;
                            
                            // ���¿γ�ѧ����
                            courses[course_index].current_students++;
                            
                            printf("ѡ�γɹ���\n");
                        } else {
                            printf("ѡ�μ�¼�������޷���ӡ�\n");
                        }
                    }
                }
                break;
            }
            case 5: {
                // �˿�
                printf("\n=== �˿� ===\n");
                printf("������Ҫ��ѡ�Ŀγ�ID: ");
                char course_id[MAX_ID_LEN];
                get_string_input("", course_id, MAX_ID_LEN);
                
                bool found = false;
                int enrollment_index = -1;
                int course_index = -1;
                
                // ����ѡ�μ�¼
                for (int i = 0; i < enrollment_count; i++) {
                    if (strcmp(enrollments[i].student_id, current_session.username) == 0 && 
                        strcmp(enrollments[i].course_id, course_id) == 0) {
                        found = true;
                        enrollment_index = i;
                        break;
                    }
                }
                
                // ���ҿγ�
                for (int i = 0; i < course_count; i++) {
                    if (strcmp(courses[i].course_id, course_id) == 0) {
                        course_index = i;
                        break;
                    }
                }
                
                if (!found) {
                    printf("��û��ѡ�����ſγ̡�\n");
                } else if (course_index == -1) {
                    printf("�γ̲����ڡ�\n");
                } else {
                    // ȷ���˿�
                    printf("��ȷ��Ҫ��ѡ�γ� %s ��(y/n): ", course_id);
                    char confirm;
                    scanf(" %c", &confirm);
                    clear_input_buffer();
                    
                    if (confirm == 'y' || confirm == 'Y') {
                        // ɾ��ѡ�μ�¼
                        for (int i = enrollment_index; i < enrollment_count - 1; i++) {
                            enrollments[i] = enrollments[i + 1];
                        }
                        enrollment_count--;
                        
                        // ���¿γ�ѧ����
                        courses[course_index].current_students--;
                        
                        printf("�˿γɹ���\n");
                    } else {
                        printf("�˿���ȡ����\n");
                    }
                }
                break;
            }
            case 6: {
                // �鿴�ɼ�
                printf("\n=== �ɼ���ѯ ===\n");
                bool found = false;
                
                for (int i = 0; i < enrollment_count; i++) {
                    if (strcmp(enrollments[i].student_id, current_session.username) == 0) {
                        // ���ҿγ���Ϣ
                        for (int j = 0; j < course_count; j++) {
                            if (strcmp(courses[j].course_id, enrollments[i].course_id) == 0) {
                                printf("�γ�ID: %s\n", courses[j].course_id);
                                printf("�γ�����: %s\n", courses[j].name);
                                printf("ѧ��: %d\n", courses[j].credits);
                                printf("ѧ��: %s\n", courses[j].semester == SPRING ? "����" : 
                                       courses[j].semester == SUMMER ? "�ļ�" : 
                                       courses[j].semester == FALL ? "�＾" : "����");
                                printf("���: %d\n", courses[j].year);
                                printf("ƽ���ɼ�: %.2f\n", enrollments[i].average);
                                printf("���״̬: %s\n", enrollments[i].completed ? "�����" : "������");
                                
                                // ��ʾ���γɼ�
                                printf("���γɼ�: ");
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
                    printf("��Ŀǰû�гɼ���¼��\n");
                }
                break;
            }
            case 7: {
                // �鿴GPA
                Student *student = NULL;
                for (int i = 0; i < student_count; i++) {
                    if (strcmp(students[i].id, current_session.username) == 0) {
                        student = &students[i];
                        break;
                    }
                }
                
                if (student) {
                    printf("\n=== GPA��Ϣ ===\n");
                    printf("��ǰGPA: %.2f\n", student->gpa);
                    printf("��ѧ��: %d\n", student->total_credits);
                    
                    // ������ϸGPA��Ϣ
                    int total_courses = 0;
                    float total_points = 0.0;
                    int total_credits_earned = 0;
                    
                    for (int i = 0; i < enrollment_count; i++) {
                        if (strcmp(enrollments[i].student_id, current_session.username) == 0 && 
                            enrollments[i].completed) {
                            // ���ҿγ���Ϣ
                            for (int j = 0; j < course_count; j++) {
                                if (strcmp(courses[j].course_id, enrollments[i].course_id) == 0) {
                                    total_courses++;
                                    total_credits_earned += courses[j].credits;
                                    
                                    // ���㼨��
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
                        printf("����ɿγ���: %d\n", total_courses);
                        printf("�ѻ��ѧ��: %d\n", total_credits_earned);
                        printf("ƽ������: %.2f\n", total_points / total_credits_earned);
                    } else {
                        printf("����û������κογ̡�\n");
                    }
                } else {
                    printf("δ�ҵ�ѧ����Ϣ��\n");
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
                printf("��Чѡ�����������롣\n");
        }
    }
}

// �ÿͲ˵�
void guest_menu() {
    int choice;
    
    while (1) {
        printf("\n=== �ÿͲ˵� ===\n");
        printf("1. �鿴�γ��б�\n");
        printf("2. �����γ�\n");
        printf("3. �鿴��ʦ�б�\n");
        printf("4. ������ʦ\n");
        printf("5. �鿴Ժϵ��Ϣ\n");
        printf("6. �������˵�\n");
        printf("��ѡ�����: ");
        
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
                printf("\n=== Ժϵ��Ϣ ===\n");
                // ͳ�Ƹ�Ժϵ��Ϣ
                char departments[MAX_DEPARTMENT_LEN][MAX_DEPARTMENT_LEN];
                int dept_count = 0;
                
                // �ռ�����Ժϵ
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
                
                // ��ʾ��Ժϵͳ����Ϣ
                for (int i = 0; i < dept_count; i++) {
                    printf("\n=== %s ===\n", departments[i]);
                    
                    // ͳ�ƿγ���
                    int course_num = 0;
                    for (int j = 0; j < course_count; j++) {
                        if (strcmp(courses[j].department, departments[i]) == 0) {
                            course_num++;
                        }
                    }
                    printf("�γ���: %d\n", course_num);
                    
                    // ͳ�ƽ�ʦ��
                    int teacher_num = 0;
                    for (int j = 0; j < teacher_count; j++) {
                        if (strcmp(teachers[j].department, departments[i]) == 0 && teachers[j].is_active) {
                            teacher_num++;
                        }
                    }
                    printf("��ʦ��: %d\n", teacher_num);
                    
                    // ͳ��ѧ����
                    int student_num = 0;
                    for (int j = 0; j < student_count; j++) {
                        if (strcmp(students[j].department, departments[i]) == 0 && students[j].is_active) {
                            student_num++;
                        }
                    }
                    printf("ѧ����: %d\n", student_num);
                }
                break;
            }
            case 6:
                return;
            default:
                printf("��Чѡ�����������롣\n");
        }
    }
}

// ���ѧ��
void add_student() {
    if (student_count >= MAX_STUDENTS) {
        printf("ѧ�������Ѵ����ޣ��޷���ӡ�\n");
        return;
    }
    
    printf("\n=== ���ѧ�� ===\n");
    
    Student new_student;
    
    // ����ѧ��
    while (1) {
        printf("ѧ��: ");
        get_string_input("", new_student.id, MAX_ID_LEN);
        
        // ���ѧ���Ƿ��Ѵ���
        bool exists = false;
        for (int i = 0; i < student_count; i++) {
            if (strcmp(students[i].id, new_student.id) == 0) {
                exists = true;
                break;
            }
        }
        
        if (exists) {
            printf("��ѧ���Ѵ��ڣ����������롣\n");
        } else if (!is_valid_id(new_student.id)) {
            printf("��Ч��ѧ�Ÿ�ʽ�����������롣\n");
        } else {
            break;
        }
    }
    
    // ��������
    printf("����: ");
    get_string_input("", new_student.name, MAX_NAME_LEN);
    
    // ��������
    printf("����: ");
    get_string_input("", new_student.password, MAX_PASSWORD_LEN);
    
    // �����Ա�
    new_student.gender = get_gender_input("�Ա� (0-��, 1-Ů, 2-����): ");
    
    // �����������
    new_student.birth_date = get_date_input("�������� (�� �� ��): ");
    
    // �����ַ
    printf("��ַ: ");
    get_string_input("", new_student.address, MAX_ADDRESS_LEN);
    
    // ����绰
    while (1) {
        printf("�绰: ");
        get_string_input("", new_student.phone, MAX_PHONE_LEN);
        
        if (is_valid_phone(new_student.phone)) {
            break;
        } else {
            printf("��Ч�ĵ绰���룬���������롣\n");
        }
    }
    
    // ��������
    while (1) {
        printf("����: ");
        get_string_input("", new_student.email, MAX_EMAIL_LEN);
        
        if (is_valid_email(new_student.email)) {
            break;
        } else {
            printf("��Ч�������ַ�����������롣\n");
        }
    }
    
    // �����꼶
    new_student.year = get_year_input("�꼶 (0-��һ, 1-���, 2-����, 3-����, 4-�о���): ");
    
    // ����Ժϵ
    printf("Ժϵ: ");
    get_string_input("", new_student.department, MAX_DEPARTMENT_LEN);
    
    // ��ʼ��GPA����ѧ��
    new_student.gpa = 0.0;
    new_student.total_credits = 0;
    
    // ������ѧ����
    new_student.admission_date = get_date_input("��ѧ���� (�� �� ��): ");
    
    // ����״̬Ϊ�ڶ�
    new_student.is_active = true;
    
    // ����༶ID
    printf("�༶ID (�����򲻷���): ");
    get_string_input("", new_student.class_id, MAX_ID_LEN);
    
    // ��ӵ�����
    students[student_count] = new_student;
    student_count++;
    
    // ��ӵ���ϣ��
    insert_hash(&student_hash, new_student.id, &students[student_count - 1]);
    
    printf("ѧ����ӳɹ���\n");
}

// �༭ѧ��
void edit_student() {
    printf("\n=== �༭ѧ�� ===\n");
    printf("������Ҫ�༭��ѧ��ѧ��: ");
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
        printf("δ�ҵ���ѧ����\n");
        return;
    }
    
    printf("\n��ǰѧ����Ϣ:\n");
    printf("1. ѧ��: %s\n", students[index].id);
    printf("2. ����: %s\n", students[index].name);
    printf("3. ����: %s\n", students[index].password);
    printf("4. �Ա�: %s\n", students[index].gender == MALE ? "��" : 
           students[index].gender == FEMALE ? "Ů" : "����");
    printf("5. ��������: %d-%d-%d\n", students[index].birth_date.year, 
           students[index].birth_date.month, students[index].birth_date.day);
    printf("6. ��ַ: %s\n", students[index].address);
    printf("7. �绰: %s\n", students[index].phone);
    printf("8. ����: %s\n", students[index].email);
    printf("9. �꼶: %s\n", students[index].year == FRESHMAN ? "��һ" : 
           students[index].year == SOPHOMORE ? "���" : 
           students[index].year == JUNIOR ? "����" : 
           students[index].year == SENIOR ? "����" : "�о���");
    printf("10. Ժϵ: %s\n", students[index].department);
    printf("11. GPA: %.2f\n", students[index].gpa);
    printf("12. ��ѧ��: %d\n", students[index].total_credits);
    printf("13. ��ѧ����: %d-%d-%d\n", students[index].admission_date.year, 
           students[index].admission_date.month, students[index].admission_date.day);
    printf("14. ״̬: %s\n", students[index].is_active ? "�ڶ�" : "��ѧ/��ҵ");
    printf("15. �༶ID: %s\n", students[index].class_id);
    
    printf("\n������Ҫ�޸ĵ��ֶα�� (0-ȡ��): ");
    int field = get_int_input("");
    
    if (field == 0) {
        printf("�޸���ȡ����\n");
        return;
    }
    
    switch (field) {
        case 1:
            printf("ѧ�Ų����޸ġ�\n");
            break;
        case 2:
            printf("������: ");
            get_string_input("", students[index].name, MAX_NAME_LEN);
            break;
        case 3:
            printf("������: ");
            get_string_input("", students[index].password, MAX_PASSWORD_LEN);
            break;
        case 4:
            students[index].gender = get_gender_input("���Ա� (0-��, 1-Ů, 2-����): ");
            break;
        case 5:
            students[index].birth_date = get_date_input("�³������� (�� �� ��): ");
            break;
        case 6:
            printf("�µ�ַ: ");
            get_string_input("", students[index].address, MAX_ADDRESS_LEN);
            break;
        case 7:
            while (1) {
                printf("�µ绰: ");
                get_string_input("", students[index].phone, MAX_PHONE_LEN);
                
                if (is_valid_phone(students[index].phone)) {
                    break;
                } else {
                    printf("��Ч�ĵ绰���룬���������롣\n");
                }
            }
            break;
        case 8:
            while (1) {
                printf("������: ");
                get_string_input("", students[index].email, MAX_EMAIL_LEN);
                
                if (is_valid_email(students[index].email)) {
                    break;
                } else {
                    printf("��Ч�������ַ�����������롣\n");
                }
            }
            break;
        case 9:
            students[index].year = get_year_input("���꼶 (0-��һ, 1-���, 2-����, 3-����, 4-�о���): ");
            break;
        case 10:
            printf("��Ժϵ: ");
            get_string_input("", students[index].department, MAX_DEPARTMENT_LEN);
            break;
        case 11:
            printf("��GPA: ");
            students[index].gpa = get_float_input("");
            break;
        case 12:
            printf("����ѧ��: ");
            students[index].total_credits = get_int_input("");
            break;
        case 13:
            students[index].admission_date = get_date_input("����ѧ���� (�� �� ��): ");
            break;
        case 14:
            printf("��״̬ (0-��ѧ/��ҵ, 1-�ڶ�): ");
            int status = get_int_input("");
            students[index].is_active = (status == 1);
            break;
        case 15:
            printf("�°༶ID: ");
            get_string_input("", students[index].class_id, MAX_ID_LEN);
            break;
        default:
            printf("��Ч���ֶα�š�\n");
            return;
    }
    
    printf("ѧ����Ϣ�Ѹ��¡�\n");
}

// ɾ��ѧ��
void delete_student() {
    printf("\n=== ɾ��ѧ�� ===\n");
    printf("������Ҫɾ����ѧ��ѧ��: ");
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
        printf("δ�ҵ���ѧ����\n");
        return;
    }
    
    // ��ʾѧ����Ϣ
    printf("\nѧ����Ϣ:\n");
    printf("ѧ��: %s\n", students[index].id);
    printf("����: %s\n", students[index].name);
    printf("Ժϵ: %s\n", students[index].department);
    printf("�꼶: %s\n", students[index].year == FRESHMAN ? "��һ" : 
           students[index].year == SOPHOMORE ? "���" : 
           students[index].year == JUNIOR ? "����" : 
           students[index].year == SENIOR ? "����" : "�о���");
    
    // ȷ��ɾ��
    printf("\nȷ��Ҫɾ����ѧ����(y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    clear_input_buffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        // �ӹ�ϣ����ɾ��
        remove_hash(&student_hash, id);
        
        // ɾ�����ѡ�μ�¼
        for (int i = enrollment_count - 1; i >= 0; i--) {
            if (strcmp(enrollments[i].student_id, id) == 0) {
                // ���¿γ�ѧ����
                for (int j = 0; j < course_count; j++) {
                    if (strcmp(courses[j].course_id, enrollments[i].course_id) == 0) {
                        courses[j].current_students--;
                        break;
                    }
                }
                
                // ɾ��ѡ�μ�¼
                for (int k = i; k < enrollment_count - 1; k++) {
                    enrollments[k] = enrollments[k + 1];
                }
                enrollment_count--;
            }
        }
        
        // ɾ��ѧ��
        for (int i = index; i < student_count - 1; i++) {
            students[i] = students[i + 1];
        }
        student_count--;
        
        printf("ѧ��ɾ���ɹ���\n");
    } else {
        printf("ɾ����ȡ����\n");
    }
}

// �鿴ѧ��
void view_student() {
    printf("\n=== �鿴ѧ�� ===\n");
    printf("������ѧ��ѧ��: ");
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
        printf("δ�ҵ���ѧ����\n");
        return;
    }
    
    printf("\n=== ѧ����Ϣ ===\n");
    printf("ѧ��: %s\n", students[index].id);
    printf("����: %s\n", students[index].name);
    printf("�Ա�: %s\n", students[index].gender == MALE ? "��" : 
           students[index].gender == FEMALE ? "Ů" : "����");
    printf("��������: %d-%d-%d\n", students[index].birth_date.year, 
           students[index].birth_date.month, students[index].birth_date.day);
    printf("��ַ: %s\n", students[index].address);
    printf("�绰: %s\n", students[index].phone);
    printf("����: %s\n", students[index].email);
    printf("�꼶: %s\n", students[index].year == FRESHMAN ? "��һ" : 
           students[index].year == SOPHOMORE ? "���" : 
           students[index].year == JUNIOR ? "����" : 
           students[index].year == SENIOR ? "����" : "�о���");
    printf("Ժϵ: %s\n", students[index].department);
    printf("GPA: %.2f\n", students[index].gpa);
    printf("��ѧ��: %d\n", students[index].total_credits);
    printf("��ѧ����: %d-%d-%d\n", students[index].admission_date.year, 
           students[index].admission_date.month, students[index].admission_date.day);
    printf("״̬: %s\n", students[index].is_active ? "�ڶ�" : "��ѧ/��ҵ");
    printf("�༶ID: %s\n", students[index].class_id);
    
    // ��ʾѡ�μ�¼
    printf("\n=== ѡ�μ�¼ ===\n");
    bool found = false;
    
    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].student_id, id) == 0) {
            // ���ҿγ���Ϣ
            for (int j = 0; j < course_count; j++) {
                if (strcmp(courses[j].course_id, enrollments[i].course_id) == 0) {
                    printf("�γ�ID: %s\n", courses[j].course_id);
                    printf("�γ�����: %s\n", courses[j].name);
                    printf("ѧ��: %d\n", courses[j].credits);
                    printf("ѧ��: %s\n", courses[j].semester == SPRING ? "����" : 
                           courses[j].semester == SUMMER ? "�ļ�" : 
                           courses[j].semester == FALL ? "�＾" : "����");
                    printf("���: %d\n", courses[j].year);
                    printf("ƽ���ɼ�: %.2f\n", enrollments[i].average);
                    printf("���״̬: %s\n", enrollments[i].completed ? "�����" : "������");
                    printf("------------------------\n");
                    found = true;
                    break;
                }
            }
        }
    }
    
    if (!found) {
        printf("��ѧ��û��ѡ�μ�¼��\n");
    }
}

// �г�����ѧ��
void list_all_students() {
    printf("\n=== ����ѧ���б� ===\n");
    
    if (student_count == 0) {
        printf("û��ѧ����¼��\n");
        return;
    }
    
    printf("%-15s %-20s %-10s %-10s %-15s %-10s %-10s %-10s\n", 
           "ѧ��", "����", "�Ա�", "�꼶", "Ժϵ", "GPA", "��ѧ��", "״̬");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < student_count; i++) {
        printf("%-15s %-20s %-10s %-10s %-15s %-10.2f %-10d %-10s\n", 
               students[i].id, 
               students[i].name, 
               students[i].gender == MALE ? "��" : 
               students[i].gender == FEMALE ? "Ů" : "����",
               students[i].year == FRESHMAN ? "��һ" : 
               students[i].year == SOPHOMORE ? "���" : 
               students[i].year == JUNIOR ? "����" : 
               students[i].year == SENIOR ? "����" : "�о���",
               students[i].department, 
               students[i].gpa, 
               students[i].total_credits, 
               students[i].is_active ? "�ڶ�" : "��ѧ/��ҵ");
    }
}

// ����ѧ��
void search_student() {
    printf("\n=== ����ѧ�� ===\n");
    printf("1. ��ѧ������\n");
    printf("2. ����������\n");
    printf("3. ��Ժϵ����\n");
    printf("4. ���༶����\n");
    printf("��ѡ��������ʽ: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1: {
            printf("������ѧ��: ");
            char id[MAX_ID_LEN];
            get_string_input("", id, MAX_ID_LEN);
            
            bool found = false;
            for (int i = 0; i < student_count; i++) {
                if (strcmp(students[i].id, id) == 0) {
                    printf("\n=== ѧ����Ϣ ===\n");
                    printf("ѧ��: %s\n", students[i].id);
                    printf("����: %s\n", students[i].name);
                    printf("�Ա�: %s\n", students[i].gender == MALE ? "��" : 
                           students[i].gender == FEMALE ? "Ů" : "����");
                    printf("��������: %d-%d-%d\n", students[i].birth_date.year, 
                           students[i].birth_date.month, students[i].birth_date.day);
                    printf("��ַ: %s\n", students[i].address);
                    printf("�绰: %s\n", students[i].phone);
                    printf("����: %s\n", students[i].email);
                    printf("�꼶: %s\n", students[i].year == FRESHMAN ? "��һ" : 
                           students[i].year == SOPHOMORE ? "���" : 
                           students[i].year == JUNIOR ? "����" : 
                           students[i].year == SENIOR ? "����" : "�о���");
                    printf("Ժϵ: %s\n", students[i].department);
                    printf("GPA: %.2f\n", students[i].gpa);
                    printf("��ѧ��: %d\n", students[i].total_credits);
                    printf("��ѧ����: %d-%d-%d\n", students[i].admission_date.year, 
                           students[i].admission_date.month, students[i].admission_date.day);
                    printf("״̬: %s\n", students[i].is_active ? "�ڶ�" : "��ѧ/��ҵ");
                    printf("�༶ID: %s\n", students[i].class_id);
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                printf("δ�ҵ�ѧ��Ϊ %s ��ѧ����\n", id);
            }
            break;
        }
        case 2: {
            printf("����������: ");
            char name[MAX_NAME_LEN];
            get_string_input("", name, MAX_NAME_LEN);
            
            bool found = false;
            printf("\n�������:\n");
            printf("%-15s %-20s %-10s %-10s %-15s %-10s %-10s %-10s\n", 
                   "ѧ��", "����", "�Ա�", "�꼶", "Ժϵ", "GPA", "��ѧ��", "״̬");
            printf("--------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < student_count; i++) {
                if (strstr(students[i].name, name) != NULL) {
                    printf("%-15s %-20s %-10s %-10s %-15s %-10.2f %-10d %-10s\n", 
                           students[i].id, 
                           students[i].name, 
                           students[i].gender == MALE ? "��" : 
                           students[i].gender == FEMALE ? "Ů" : "����",
                           students[i].year == FRESHMAN ? "��һ" : 
                           students[i].year == SOPHOMORE ? "���" : 
                           students[i].year == JUNIOR ? "����" : 
                           students[i].year == SENIOR ? "����" : "�о���",
                           students[i].department, 
                           students[i].gpa, 
                           students[i].total_credits, 
                           students[i].is_active ? "�ڶ�" : "��ѧ/��ҵ");
                    found = true;
                }
            }
            
            if (!found) {
                printf("δ�ҵ��������� %s ��ѧ����\n", name);
            }
            break;
        }
        case 3: {
            printf("������Ժϵ: ");
            char department[MAX_DEPARTMENT_LEN];
            get_string_input("", department, MAX_DEPARTMENT_LEN);
            
            bool found = false;
            printf("\n�������:\n");
            printf("%-15s %-20s %-10s %-10s %-15s %-10s %-10s %-10s\n", 
                   "ѧ��", "����", "�Ա�", "�꼶", "Ժϵ", "GPA", "��ѧ��", "״̬");
            printf("--------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < student_count; i++) {
                if (strstr(students[i].department, department) != NULL) {
                    printf("%-15s %-20s %-10s %-10s %-15s %-10.2f %-10d %-10s\n", 
                           students[i].id, 
                           students[i].name, 
                           students[i].gender == MALE ? "��" : 
                           students[i].gender == FEMALE ? "Ů" : "����",
                           students[i].year == FRESHMAN ? "��һ" : 
                           students[i].year == SOPHOMORE ? "���" : 
                           students[i].year == JUNIOR ? "����" : 
                           students[i].year == SENIOR ? "����" : "�о���",
                           students[i].department, 
                           students[i].gpa, 
                           students[i].total_credits, 
                           students[i].is_active ? "�ڶ�" : "��ѧ/��ҵ");
                    found = true;
                }
            }
            
            if (!found) {
                printf("δ�ҵ�ԺϵΪ %s ��ѧ����\n", department);
            }
            break;
        }
        case 4: {
            printf("������༶ID: ");
            char class_id[MAX_ID_LEN];
            get_string_input("", class_id, MAX_ID_LEN);
            
            bool found = false;
            printf("\n�������:\n");
            printf("%-15s %-20s %-10s %-10s %-15s %-10s %-10s %-10s\n", 
                   "ѧ��", "����", "�Ա�", "�꼶", "Ժϵ", "GPA", "��ѧ��", "״̬");
            printf("--------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < student_count; i++) {
                if (strcmp(students[i].class_id, class_id) == 0) {
                    printf("%-15s %-20s %-10s %-10s %-15s %-10.2f %-10d %-10s\n", 
                           students[i].id, 
                           students[i].name, 
                           students[i].gender == MALE ? "��" : 
                           students[i].gender == FEMALE ? "Ů" : "����",
                           students[i].year == FRESHMAN ? "��һ" : 
                           students[i].year == SOPHOMORE ? "���" : 
                           students[i].year == JUNIOR ? "����" : 
                           students[i].year == SENIOR ? "����" : "�о���",
                           students[i].department, 
                           students[i].gpa, 
                           students[i].total_credits, 
                           students[i].is_active ? "�ڶ�" : "��ѧ/��ҵ");
                    found = true;
                }
            }
            
            if (!found) {
                printf("δ�ҵ��༶Ϊ %s ��ѧ����\n", class_id);
            }
            break;
        }
        default:
            printf("��Чѡ��\n");
    }
}

// ����ѧ��
void sort_students() {
    printf("\n=== ����ѧ�� ===\n");
    printf("1. ��ѧ������\n");
    printf("2. ����������\n");
    printf("3. ��GPA����\n");
    printf("4. ����ѧ������\n");
    printf("��ѡ������ʽ: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1:
            qsort(students, student_count, sizeof(Student), compare_students_by_id);
            printf("�Ѱ�ѧ������\n");
            break;
        case 2:
            qsort(students, student_count, sizeof(Student), compare_students_by_name);
            printf("�Ѱ���������\n");
            break;
        case 3:
            qsort(students, student_count, sizeof(Student), compare_students_by_gpa);
            printf("�Ѱ�GPA����\n");
            break;
        case 4:
            // ����ѧ��������Ҫ�Զ���ȽϺ���
            for (int i = 0; i < student_count - 1; i++) {
                for (int j = 0; j < student_count - i - 1; j++) {
                    if (students[j].total_credits < students[j + 1].total_credits) {
                        Student temp = students[j];
                        students[j] = students[j + 1];
                        students[j + 1] = temp;
                    }
                }
            }
            printf("�Ѱ���ѧ������\n");
            break;
        default:
            printf("��Чѡ��\n");
            return;
    }
    
    // ��ʾ�����Ľ��
    list_all_students();
}

// ��ӿγ�
void add_course() {
    if (course_count >= MAX_COURSES) {
        printf("�γ������Ѵ����ޣ��޷���ӡ�\n");
        return;
    }
    
    printf("\n=== ��ӿγ� ===\n");
    
    Course new_course;
    
    // ����γ�ID
    while (1) {
        printf("�γ�ID: ");
        get_string_input("", new_course.course_id, MAX_ID_LEN);
        
        // ���γ�ID�Ƿ��Ѵ���
        bool exists = false;
        for (int i = 0; i < course_count; i++) {
            if (strcmp(courses[i].course_id, new_course.course_id) == 0) {
                exists = true;
                break;
            }
        }
        
        if (exists) {
            printf("�ÿγ�ID�Ѵ��ڣ����������롣\n");
        } else if (!is_valid_id(new_course.course_id)) {
            printf("��Ч�Ŀγ�ID��ʽ�����������롣\n");
        } else {
            break;
        }
    }
    
    // ����γ�����
    printf("�γ�����: ");
    get_string_input("", new_course.name, MAX_COURSE_NAME_LEN);
    
    // ����ѧ��
    printf("ѧ��: ");
    new_course.credits = get_int_input("");
    
    // ����Ժϵ
    printf("Ժϵ: ");
    get_string_input("", new_course.department, MAX_DEPARTMENT_LEN);
    
    // �����ʦID
    printf("��ʦID: ");
    get_string_input("", new_course.teacher_id, MAX_ID_LEN);
    
    // ����ѧ��
    new_course.semester = get_semester_input("ѧ�� (0-����, 1-�ļ�, 2-�＾, 3-����): ");
    
    // �������
    printf("���: ");
    new_course.year = get_int_input("");
    
    // �������ѧ����
    printf("���ѧ����: ");
    new_course.max_students = get_int_input("");
    
    // ��ʼ����ǰѧ����
    new_course.current_students = 0;
    
    // ��ӵ�����
    courses[course_count] = new_course;
    course_count++;
    
    // ��ӵ���ϣ��
    insert_hash(&course_hash, new_course.course_id, &courses[course_count - 1]);
    
    printf("�γ���ӳɹ���\n");
}

// �༭�γ�
void edit_course() {
    printf("\n=== �༭�γ� ===\n");
    printf("������Ҫ�༭�Ŀγ�ID: ");
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
        printf("δ�ҵ��ÿγ̡�\n");
        return;
    }
    
    printf("\n��ǰ�γ���Ϣ:\n");
    printf("1. �γ�ID: %s\n", courses[index].course_id);
    printf("2. �γ�����: %s\n", courses[index].name);
    printf("3. ѧ��: %d\n", courses[index].credits);
    printf("4. Ժϵ: %s\n", courses[index].department);
    printf("5. ��ʦID: %s\n", courses[index].teacher_id);
    printf("6. ѧ��: %s\n", courses[index].semester == SPRING ? "����" : 
           courses[index].semester == SUMMER ? "�ļ�" : 
           courses[index].semester == FALL ? "�＾" : "����");
    printf("7. ���: %d\n", courses[index].year);
    printf("8. ���ѧ����: %d\n", courses[index].max_students);
    printf("9. ��ǰѧ����: %d\n", courses[index].current_students);
    
    printf("\n������Ҫ�޸ĵ��ֶα�� (0-ȡ��): ");
    int field = get_int_input("");
    
    if (field == 0) {
        printf("�޸���ȡ����\n");
        return;
    }
    
    switch (field) {
        case 1:
            printf("�γ�ID�����޸ġ�\n");
            break;
        case 2:
            printf("�¿γ�����: ");
            get_string_input("", courses[index].name, MAX_COURSE_NAME_LEN);
            break;
        case 3:
            printf("��ѧ��: ");
            courses[index].credits = get_int_input("");
            break;
        case 4:
            printf("��Ժϵ: ");
            get_string_input("", courses[index].department, MAX_DEPARTMENT_LEN);
            break;
        case 5:
            printf("�½�ʦID: ");
            get_string_input("", courses[index].teacher_id, MAX_ID_LEN);
            break;
        case 6:
            courses[index].semester = get_semester_input("��ѧ�� (0-����, 1-�ļ�, 2-�＾, 3-����): ");
            break;
        case 7:
            printf("�����: ");
            courses[index].year = get_int_input("");
            break;
        case 8:
            printf("�����ѧ����: ");
            int new_max = get_int_input("");
            if (new_max < courses[index].current_students) {
                printf("����: ���ѧ��������С�ڵ�ǰѧ���� (%d)��\n", courses[index].current_students);
                printf("�Ƿ����? (y/n): ");
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
            printf("��ǰѧ��������ֱ���޸ġ�\n");
            break;
        default:
            printf("��Ч���ֶα�š�\n");
            return;
    }
    
    printf("�γ���Ϣ�Ѹ��¡�\n");
}

// ɾ���γ�
void delete_course() {
    printf("\n=== ɾ���γ� ===\n");
    printf("������Ҫɾ���Ŀγ�ID: ");
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
        printf("δ�ҵ��ÿγ̡�\n");
        return;
    }
    
    // ��ʾ�γ���Ϣ
    printf("\n�γ���Ϣ:\n");
    printf("�γ�ID: %s\n", courses[index].course_id);
    printf("�γ�����: %s\n", courses[index].name);
    printf("ѧ��: %d\n", courses[index].credits);
    printf("Ժϵ: %s\n", courses[index].department);
    printf("��ʦID: %s\n", courses[index].teacher_id);
    printf("ѧ��: %s\n", courses[index].semester == SPRING ? "����" : 
           courses[index].semester == SUMMER ? "�ļ�" : 
           courses[index].semester == FALL ? "�＾" : "����");
    printf("���: %d\n", courses[index].year);
    printf("��ǰѧ����: %d\n", courses[index].current_students);
    
    // ȷ��ɾ��
    printf("\nȷ��Ҫɾ���ÿγ���(y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    clear_input_buffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        // �ӹ�ϣ����ɾ��
        remove_hash(&course_hash, id);
        
        // ɾ�����ѡ�μ�¼
        for (int i = enrollment_count - 1; i >= 0; i--) {
            if (strcmp(enrollments[i].course_id, id) == 0) {
                // ɾ��ѡ�μ�¼
                for (int j = i; j < enrollment_count - 1; j++) {
                    enrollments[j] = enrollments[j + 1];
                }
                enrollment_count--;
            }
        }
        
        // ɾ���γ�
        for (int i = index; i < course_count - 1; i++) {
            courses[i] = courses[i + 1];
        }
        course_count--;
        
        printf("�γ�ɾ���ɹ���\n");
    } else {
        printf("ɾ����ȡ����\n");
    }
}

// �鿴�γ�
void view_course() {
    printf("\n=== �鿴�γ� ===\n");
    printf("������γ�ID: ");
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
        printf("δ�ҵ��ÿγ̡�\n");
        return;
    }
    
    printf("\n=== �γ���Ϣ ===\n");
    printf("�γ�ID: %s\n", courses[index].course_id);
    printf("�γ�����: %s\n", courses[index].name);
    printf("ѧ��: %d\n", courses[index].credits);
    printf("Ժϵ: %s\n", courses[index].department);
    printf("��ʦID: %s\n", courses[index].teacher_id);
    printf("ѧ��: %s\n", courses[index].semester == SPRING ? "����" : 
           courses[index].semester == SUMMER ? "�ļ�" : 
           courses[index].semester == FALL ? "�＾" : "����");
    printf("���: %d\n", courses[index].year);
    printf("���ѧ����: %d\n", courses[index].max_students);
    printf("��ǰѧ����: %d\n", courses[index].current_students);
    
    // ��ʾѡ��ѧ��
    printf("\n=== ѡ��ѧ�� ===\n");
    bool found = false;
    
    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].course_id, id) == 0) {
            // ����ѧ����Ϣ
            for (int j = 0; j < student_count; j++) {
                if (strcmp(students[j].id, enrollments[i].student_id) == 0) {
                    printf("ѧ��: %s\n", students[j].id);
                    printf("����: %s\n", students[j].name);
                    printf("Ժϵ: %s\n", students[j].department);
                    printf("�꼶: %s\n", students[j].year == FRESHMAN ? "��һ" : 
                           students[j].year == SOPHOMORE ? "���" : 
                           students[j].year == JUNIOR ? "����" : 
                           students[j].year == SENIOR ? "����" : "�о���");
                    printf("ƽ���ɼ�: %.2f\n", enrollments[i].average);
                    printf("���״̬: %s\n", enrollments[i].completed ? "�����" : "������");
                    printf("------------------------\n");
                    found = true;
                    break;
                }
            }
        }
    }
    
    if (!found) {
        printf("�ÿγ�û��ѡ��ѧ����\n");
    }
}

// �г����пγ�
void list_all_courses() {
    printf("\n=== ���пγ��б� ===\n");
    
    if (course_count == 0) {
        printf("û�пγ̼�¼��\n");
        return;
    }
    
    printf("%-15s %-30s %-10s %-15s %-15s %-10s %-10s %-15s %-15s\n", 
           "�γ�ID", "�γ�����", "ѧ��", "Ժϵ", "��ʦID", "ѧ��", "���", "���ѧ����", "��ǰѧ����");
    printf("------------------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < course_count; i++) {
        printf("%-15s %-30s %-10d %-15s %-15s %-10s %-10d %-15d %-15d\n", 
               courses[i].course_id, 
               courses[i].name, 
               courses[i].credits, 
               courses[i].department, 
               courses[i].teacher_id, 
               courses[i].semester == SPRING ? "����" : 
               courses[i].semester == SUMMER ? "�ļ�" : 
               courses[i].semester == FALL ? "�＾" : "����",
               courses[i].year, 
               courses[i].max_students, 
               courses[i].current_students);
    }
}

// �����γ�
void search_course() {
    printf("\n=== �����γ� ===\n");
    printf("1. ���γ�ID����\n");
    printf("2. ���γ���������\n");
    printf("3. ��Ժϵ����\n");
    printf("4. ����ʦID����\n");
    printf("��ѡ��������ʽ: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1: {
            printf("������γ�ID: ");
            char id[MAX_ID_LEN];
            get_string_input("", id, MAX_ID_LEN);
            
            bool found = false;
            for (int i = 0; i < course_count; i++) {
                if (strcmp(courses[i].course_id, id) == 0) {
                    printf("\n=== �γ���Ϣ ===\n");
                    printf("�γ�ID: %s\n", courses[i].course_id);
                    printf("�γ�����: %s\n", courses[i].name);
                    printf("ѧ��: %d\n", courses[i].credits);
                    printf("Ժϵ: %s\n", courses[i].department);
                    printf("��ʦID: %s\n", courses[i].teacher_id);
                    printf("ѧ��: %s\n", courses[i].semester == SPRING ? "����" : 
                           courses[i].semester == SUMMER ? "�ļ�" : 
                           courses[i].semester == FALL ? "�＾" : "����");
                    printf("���: %d\n", courses[i].year);
                    printf("���ѧ����: %d\n", courses[i].max_students);
                    printf("��ǰѧ����: %d\n", courses[i].current_students);
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                printf("δ�ҵ��γ�IDΪ %s �Ŀγ̡�\n", id);
            }
            break;
        }
        case 2: {
            printf("������γ�����: ");
            char name[MAX_COURSE_NAME_LEN];
            get_string_input("", name, MAX_COURSE_NAME_LEN);
            
            bool found = false;
            printf("\n�������:\n");
            printf("%-15s %-30s %-10s %-15s %-15s %-10s %-10s %-15s %-15s\n", 
                   "�γ�ID", "�γ�����", "ѧ��", "Ժϵ", "��ʦID", "ѧ��", "���", "���ѧ����", "��ǰѧ����");
            printf("------------------------------------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < course_count; i++) {
                if (strstr(courses[i].name, name) != NULL) {
                    printf("%-15s %-30s %-10d %-15s %-15s %-10s %-10d %-15d %-15d\n", 
                           courses[i].course_id, 
                           courses[i].name, 
                           courses[i].credits, 
                           courses[i].department, 
                           courses[i].teacher_id, 
                           courses[i].semester == SPRING ? "����" : 
                           courses[i].semester == SUMMER ? "�ļ�" : 
                           courses[i].semester == FALL ? "�＾" : "����",
                           courses[i].year, 
                           courses[i].max_students, 
                           courses[i].current_students);
                    found = true;
                }
            }
            
            if (!found) {
                printf("δ�ҵ��γ����ư��� %s �Ŀγ̡�\n", name);
            }
            break;
        }
        case 3: {
            printf("������Ժϵ: ");
            char department[MAX_DEPARTMENT_LEN];
            get_string_input("", department, MAX_DEPARTMENT_LEN);
            
            bool found = false;
            printf("\n�������:\n");
            printf("%-15s %-30s %-10s %-15s %-15s %-10s %-10s %-15s %-15s\n", 
                   "�γ�ID", "�γ�����", "ѧ��", "Ժϵ", "��ʦID", "ѧ��", "���", "���ѧ����", "��ǰѧ����");
            printf("------------------------------------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < course_count; i++) {
                if (strstr(courses[i].department, department) != NULL) {
                    printf("%-15s %-30s %-10d %-15s %-15s %-10s %-10d %-15d %-15d\n", 
                           courses[i].course_id, 
                           courses[i].name, 
                           courses[i].credits, 
                           courses[i].department, 
                           courses[i].teacher_id, 
                           courses[i].semester == SPRING ? "����" : 
                           courses[i].semester == SUMMER ? "�ļ�" : 
                           courses[i].semester == FALL ? "�＾" : "����",
                           courses[i].year, 
                           courses[i].max_students, 
                           courses[i].current_students);
                    found = true;
                }
            }
            
            if (!found) {
                printf("δ�ҵ�ԺϵΪ %s �Ŀγ̡�\n", department);
            }
            break;
        }
        case 4: {
            printf("�������ʦID: ");
            char teacher_id[MAX_ID_LEN];
            get_string_input("", teacher_id, MAX_ID_LEN);
            
            bool found = false;
            printf("\n�������:\n");
            printf("%-15s %-30s %-10s %-15s %-15s %-10s %-10s %-15s %-15s\n", 
                   "�γ�ID", "�γ�����", "ѧ��", "Ժϵ", "��ʦID", "ѧ��", "���", "���ѧ����", "��ǰѧ����");
            printf("------------------------------------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < course_count; i++) {
                if (strcmp(courses[i].teacher_id, teacher_id) == 0) {
                    printf("%-15s %-30s %-10d %-15s %-15s %-10s %-10d %-15d %-15d\n", 
                           courses[i].course_id, 
                           courses[i].name, 
                           courses[i].credits, 
                           courses[i].department, 
                           courses[i].teacher_id, 
                           courses[i].semester == SPRING ? "����" : 
                           courses[i].semester == SUMMER ? "�ļ�" : 
                           courses[i].semester == FALL ? "�＾" : "����",
                           courses[i].year, 
                           courses[i].max_students, 
                           courses[i].current_students);
                    found = true;
                }
            }
            
            if (!found) {
                printf("δ�ҵ���ʦIDΪ %s �Ŀγ̡�\n", teacher_id);
            }
            break;
        }
        default:
            printf("��Чѡ��\n");
    }
}

// ����γ�
void sort_courses() {
    printf("\n=== ����γ� ===\n");
    printf("1. ���γ�ID����\n");
    printf("2. ���γ���������\n");
    printf("3. ��ѧ������\n");
    printf("4. ����ǰѧ��������\n");
    printf("��ѡ������ʽ: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1:
            qsort(courses, course_count, sizeof(Course), compare_courses_by_id);
            printf("�Ѱ��γ�ID����\n");
            break;
        case 2:
            qsort(courses, course_count, sizeof(Course), compare_courses_by_name);
            printf("�Ѱ��γ���������\n");
            break;
        case 3:
            qsort(courses, course_count, sizeof(Course), compare_courses_by_credits);
            printf("�Ѱ�ѧ������\n");
            break;
        case 4:
            // ����ǰѧ����������Ҫ�Զ���ȽϺ���
            for (int i = 0; i < course_count - 1; i++) {
                for (int j = 0; j < course_count - i - 1; j++) {
                    if (courses[j].current_students < courses[j + 1].current_students) {
                        Course temp = courses[j];
                        courses[j] = courses[j + 1];
                        courses[j + 1] = temp;
                    }
                }
            }
            printf("�Ѱ���ǰѧ��������\n");
            break;
        default:
            printf("��Чѡ��\n");
            return;
    }
    
    // ��ʾ�����Ľ��
    list_all_courses();
}

// ѧ��ѡ��
void enroll_student() {
    printf("\n=== ѧ��ѡ�� ===\n");
    printf("������ѧ��ѧ��: ");
    char student_id[MAX_ID_LEN];
    get_string_input("", student_id, MAX_ID_LEN);
    
    // ���ѧ���Ƿ����
    bool student_exists = false;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, student_id) == 0 && students[i].is_active) {
            student_exists = true;
            break;
        }
    }
    
    if (!student_exists) {
        printf("ѧ�������ڻ�����ѧ/��ҵ��\n");
        return;
    }
    
    printf("������γ�ID: ");
    char course_id[MAX_ID_LEN];
    get_string_input("", course_id, MAX_ID_LEN);
    
    // ���γ��Ƿ����
    int course_index = -1;
    for (int i = 0; i < course_count; i++) {
        if (strcmp(courses[i].course_id, course_id) == 0) {
            course_index = i;
            break;
        }
    }
    
    if (course_index == -1) {
        printf("�γ̲����ڡ�\n");
        return;
    }
    
    // ���γ��Ƿ�����
    if (courses[course_index].current_students >= courses[course_index].max_students) {
        printf("�ÿγ�����Ա��\n");
        return;
    }
    
    // ���ѧ���Ƿ��Ѿ�ѡ�����ſ�
    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].student_id, student_id) == 0 && 
            strcmp(enrollments[i].course_id, course_id) == 0) {
            printf("��ѧ���Ѿ�ѡ�������ſγ̡�\n");
            return;
        }
    }
    
    // ���ѡ�μ�¼
    if (enrollment_count < MAX_ENROLLMENTS) {
        strcpy(enrollments[enrollment_count].student_id, student_id);
        strcpy(enrollments[enrollment_count].course_id, course_id);
        
        // ��ʼ���ɼ�
        for (int i = 0; i < MAX_GRADES; i++) {
            enrollments[enrollment_count].grades[i] = F;
        }
        enrollments[enrollment_count].average = 0.0;
        
        // ����ѡ������Ϊ��ǰ����
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        enrollments[enrollment_count].enrollment_date.day = tm->tm_mday;
        enrollments[enrollment_count].enrollment_date.month = tm->tm_mon + 1;
        enrollments[enrollment_count].enrollment_date.year = tm->tm_year + 1900;
        
        enrollments[enrollment_count].completed = false;
        
        enrollment_count++;
        
        // ���¿γ�ѧ����
        courses[course_index].current_students++;
        
        printf("ѡ�γɹ���\n");
    } else {
        printf("ѡ�μ�¼�������޷���ӡ�\n");
    }
}

// ѧ���˿�
void drop_course() {
    printf("\n=== ѧ���˿� ===\n");
    printf("������ѧ��ѧ��: ");
    char student_id[MAX_ID_LEN];
    get_string_input("", student_id, MAX_ID_LEN);
    
    // ���ѧ���Ƿ����
    bool student_exists = false;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, student_id) == 0) {
            student_exists = true;
            break;
        }
    }
    
    if (!student_exists) {
        printf("ѧ�������ڡ�\n");
        return;
    }
    
    printf("������γ�ID: ");
    char course_id[MAX_ID_LEN];
    get_string_input("", course_id, MAX_ID_LEN);
    
    // ����ѡ�μ�¼
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
        printf("��ѧ��û��ѡ�����ſγ̡�\n");
        return;
    }
    
    if (course_index == -1) {
        printf("�γ̲����ڡ�\n");
        return;
    }
    
    // ȷ���˿�
    printf("ȷ��ҪΪѧ�� %s ��ѡ�γ� %s ��(y/n): ", student_id, course_id);
    char confirm;
    scanf(" %c", &confirm);
    clear_input_buffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        // ɾ��ѡ�μ�¼
        for (int i = enrollment_index; i < enrollment_count - 1; i++) {
            enrollments[i] = enrollments[i + 1];
        }
        enrollment_count--;
        
        // ���¿γ�ѧ����
        courses[course_index].current_students--;
        
        printf("�˿γɹ���\n");
    } else {
        printf("�˿���ȡ����\n");
    }
}

// �鿴ѡ�μ�¼
void view_enrollments() {
    printf("\n=== ѡ�μ�¼ ===\n");
    printf("1. �鿴����ѡ�μ�¼\n");
    printf("2. ��ѧ���鿴\n");
    printf("3. ���γ̲鿴\n");
    printf("��ѡ��鿴��ʽ: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1:
            if (enrollment_count == 0) {
                printf("û��ѡ�μ�¼��\n");
                return;
            }
            
            printf("%-15s %-15s %-10s %-10s %-15s %-10s\n", 
                   "ѧ��ID", "�γ�ID", "ƽ���ɼ�", "���״̬", "ѡ������", "���γɼ�");
            printf("----------------------------------------------------------------\n");
            
            for (int i = 0; i < enrollment_count; i++) {
                printf("%-15s %-15s %-10.2f %-10s %-15d-%d-%d ", 
                       enrollments[i].student_id, 
                       enrollments[i].course_id, 
                       enrollments[i].average, 
                       enrollments[i].completed ? "�����" : "������",
                       enrollments[i].enrollment_date.year,
                       enrollments[i].enrollment_date.month,
                       enrollments[i].enrollment_date.day);
                
                // ��ʾ���γɼ�
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
            printf("������ѧ��ѧ��: ");
            char student_id[MAX_ID_LEN];
            get_string_input("", student_id, MAX_ID_LEN);
            
            bool found = false;
            printf("\nѧ�� %s ��ѡ�μ�¼:\n", student_id);
            printf("%-15s %-15s %-10s %-10s %-15s %-10s\n", 
                   "ѧ��ID", "�γ�ID", "ƽ���ɼ�", "���״̬", "ѡ������", "���γɼ�");
            printf("----------------------------------------------------------------\n");
            
            for (int i = 0; i < enrollment_count; i++) {
                if (strcmp(enrollments[i].student_id, student_id) == 0) {
                    printf("%-15s %-15s %-10.2f %-10s %-15d-%d-%d ", 
                           enrollments[i].student_id, 
                           enrollments[i].course_id, 
                           enrollments[i].average, 
                           enrollments[i].completed ? "�����" : "������",
                           enrollments[i].enrollment_date.year,
                           enrollments[i].enrollment_date.month,
                           enrollments[i].enrollment_date.day);
                    
                    // ��ʾ���γɼ�
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
                printf("��ѧ��û��ѡ�μ�¼��\n");
            }
            break;
        }
        case 3: {
            printf("������γ�ID: ");
            char course_id[MAX_ID_LEN];
            get_string_input("", course_id, MAX_ID_LEN);
            
            bool found = false;
            printf("\n�γ� %s ��ѡ�μ�¼:\n", course_id);
            printf("%-15s %-15s %-10s %-10s %-15s %-10s\n", 
                   "ѧ��ID", "�γ�ID", "ƽ���ɼ�", "���״̬", "ѡ������", "���γɼ�");
            printf("----------------------------------------------------------------\n");
            
            for (int i = 0; i < enrollment_count; i++) {
                if (strcmp(enrollments[i].course_id, course_id) == 0) {
                    printf("%-15s %-15s %-10.2f %-10s %-15d-%d-%d ", 
                           enrollments[i].student_id, 
                           enrollments[i].course_id, 
                           enrollments[i].average, 
                           enrollments[i].completed ? "�����" : "������",
                           enrollments[i].enrollment_date.year,
                           enrollments[i].enrollment_date.month,
                           enrollments[i].enrollment_date.day);
                    
                    // ��ʾ���γɼ�
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
                printf("�ÿγ�û��ѡ�μ�¼��\n");
            }
            break;
        }
        default:
            printf("��Чѡ��\n");
    }
}

// ����ɼ�
void assign_grade() {
    printf("\n=== ����ɼ� ===\n");
    printf("������ѧ��ѧ��: ");
    char student_id[MAX_ID_LEN];
    get_string_input("", student_id, MAX_ID_LEN);
    
    printf("������γ�ID: ");
    char course_id[MAX_ID_LEN];
    get_string_input("", course_id, MAX_ID_LEN);
    
    // ����ѡ�μ�¼
    int enrollment_index = -1;
    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].student_id, student_id) == 0 && 
            strcmp(enrollments[i].course_id, course_id) == 0) {
            enrollment_index = i;
            break;
        }
    }
    
    if (enrollment_index == -1) {
        printf("δ�ҵ���ѧ����ѡ�μ�¼��\n");
        return;
    }
    
    // ��ʾ��ǰ�ɼ�
    printf("\n��ǰ�ɼ���¼:\n");
    printf("ƽ���ɼ�: %.2f\n", enrollments[enrollment_index].average);
    printf("���γɼ�: ");
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
    
    // �����µĳɼ�
    printf("\n�������µĳɼ� (����-1����):\n");
    float total = 0.0;
    int count = 0;
    
    for (int i = 0; i < MAX_GRADES; i++) {
        printf("�ɼ� %d (0-100): ", i+1);
        float score = get_float_input("");
        
        if (score < 0) {
            break;
        }
        
        if (score > 100) {
            printf("�ɼ����ܳ���100�����������롣\n");
            i--;
            continue;
        }
        
        // ת��Ϊ�ȼ�
        if (score >= 90) enrollments[enrollment_index].grades[i] = A;
        else if (score >= 80) enrollments[enrollment_index].grades[i] = B;
        else if (score >= 70) enrollments[enrollment_index].grades[i] = C;
        else if (score >= 60) enrollments[enrollment_index].grades[i] = D;
        else enrollments[enrollment_index].grades[i] = F;
        
        total += score;
        count++;
    }
    
    if (count > 0) {
        // ����ƽ���ɼ�
        enrollments[enrollment_index].average = total / count;
        
        // ����Ƿ���ɿγ�
        if (enrollments[enrollment_index].average >= 60) {
            enrollments[enrollment_index].completed = true;
            
            // ����ѧ����GPA����ѧ��
            for (int i = 0; i < student_count; i++) {
                if (strcmp(students[i].id, student_id) == 0) {
                    // ���ҿγ�ѧ��
                    int credits = 0;
                    for (int j = 0; j < course_count; j++) {
                        if (strcmp(courses[j].course_id, course_id) == 0) {
                            credits = courses[j].credits;
                            break;
                        }
                    }
                    
                    // ���㼨��
                    float grade_point = 0.0;
                    if (enrollments[enrollment_index].average >= 90) grade_point = 4.0;
                    else if (enrollments[enrollment_index].average >= 80) grade_point = 3.0;
                    else if (enrollments[enrollment_index].average >= 70) grade_point = 2.0;
                    else if (enrollments[enrollment_index].average >= 60) grade_point = 1.0;
                    else grade_point = 0.0;
                    
                    // ����GPA
                    float total_points = students[i].gpa * students[i].total_credits;
                    students[i].total_credits += credits;
                    students[i].gpa = (total_points + grade_point * credits) / students[i].total_credits;
                    
                    break;
                }
            }
        }
        
        printf("�ɼ�����ɹ���\n");
        printf("�µ�ƽ���ɼ�: %.2f\n", enrollments[enrollment_index].average);
        printf("���״̬: %s\n", enrollments[enrollment_index].completed ? "�����" : "������");
    } else {
        printf("û�������µĳɼ���\n");
    }
}

// ����GPA
void calculate_gpa() {
    printf("\n=== ����GPA ===\n");
    printf("������ѧ��ѧ��: ");
    char student_id[MAX_ID_LEN];
    get_string_input("", student_id, MAX_ID_LEN);
    
    // ����ѧ��
    int student_index = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, student_id) == 0) {
            student_index = i;
            break;
        }
    }
    
    if (student_index == -1) {
        printf("δ�ҵ���ѧ����\n");
        return;
    }
    
    // ����GPA
    float total_points = 0.0;
    int total_credits = 0;
    
    for (int i = 0; i < enrollment_count; i++) {
        if (strcmp(enrollments[i].student_id, student_id) == 0 && 
            enrollments[i].completed) {
            // ���ҿγ�ѧ��
            int credits = 0;
            for (int j = 0; j < course_count; j++) {
                if (strcmp(courses[j].course_id, enrollments[i].course_id) == 0) {
                    credits = courses[j].credits;
                    break;
                }
            }
            
            // ���㼨��
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
        
        printf("\nGPA������:\n");
        printf("ѧ��: %s\n", students[student_index].name);
        printf("�ܼ���: %.2f\n", total_points);
        printf("��ѧ��: %d\n", total_credits);
        printf("GPA: %.2f\n", students[student_index].gpa);
    } else {
        printf("��ѧ��û������κογ̣��޷�����GPA��\n");
    }
}

// ��ӽ�ʦ
void add_teacher() {
    if (teacher_count >= MAX_TEACHERS) {
        printf("��ʦ�����Ѵ����ޣ��޷���ӡ�\n");
        return;
    }
    
    printf("\n=== ��ӽ�ʦ ===\n");
    
    Teacher new_teacher;
    
    // �����ʦID
    while (1) {
        printf("��ʦID: ");
        get_string_input("", new_teacher.id, MAX_ID_LEN);
        
        // ����ʦID�Ƿ��Ѵ���
        bool exists = false;
        for (int i = 0; i < teacher_count; i++) {
            if (strcmp(teachers[i].id, new_teacher.id) == 0) {
                exists = true;
                break;
            }
        }
        
        if (exists) {
            printf("�ý�ʦID�Ѵ��ڣ����������롣\n");
        } else if (!is_valid_id(new_teacher.id)) {
            printf("��Ч�Ľ�ʦID��ʽ�����������롣\n");
        } else {
            break;
        }
    }
    
    // ��������
    printf("����: ");
    get_string_input("", new_teacher.name, MAX_NAME_LEN);
    
    // ��������
    printf("����: ");
    get_string_input("", new_teacher.password, MAX_PASSWORD_LEN);
    
    // �����Ա�
    new_teacher.gender = get_gender_input("�Ա� (0-��, 1-Ů, 2-����): ");
    
    // �����������
    new_teacher.birth_date = get_date_input("�������� (�� �� ��): ");
    
    // ����Ժϵ
    printf("Ժϵ: ");
    get_string_input("", new_teacher.department, MAX_DEPARTMENT_LEN);
    
    // ����绰
    while (1) {
        printf("�绰: ");
        get_string_input("", new_teacher.phone, MAX_PHONE_LEN);
        
        if (is_valid_phone(new_teacher.phone)) {
            break;
        } else {
            printf("��Ч�ĵ绰���룬���������롣\n");
        }
    }
    
    // ��������
    while (1) {
        printf("����: ");
        get_string_input("", new_teacher.email, MAX_EMAIL_LEN);
        
        if (is_valid_email(new_teacher.email)) {
            break;
        } else {
            printf("��Ч�������ַ�����������롣\n");
        }
    }
    
    // ����Ƹ������
    new_teacher.hire_date = get_date_input("Ƹ������ (�� �� ��): ");
    
    // ����ְ��
    printf("ְ��: ");
    get_string_input("", new_teacher.title, MAX_NAME_LEN);
    
    // ����н��
    printf("н��: ");
    new_teacher.salary = get_float_input("");
    
    // ����״̬Ϊ��ְ
    new_teacher.is_active = true;
    
    // ��ӵ�����
    teachers[teacher_count] = new_teacher;
    teacher_count++;
    
    // ��ӵ���ϣ��
    insert_hash(&teacher_hash, new_teacher.id, &teachers[teacher_count - 1]);
    
    printf("��ʦ��ӳɹ���\n");
}

// �༭��ʦ
void edit_teacher() {
    printf("\n=== �༭��ʦ ===\n");
    printf("������Ҫ�༭�Ľ�ʦID: ");
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
        printf("δ�ҵ��ý�ʦ��\n");
        return;
    }
    
    printf("\n��ǰ��ʦ��Ϣ:\n");
    printf("1. ��ʦID: %s\n", teachers[index].id);
    printf("2. ����: %s\n", teachers[index].name);
    printf("3. ����: %s\n", teachers[index].password);
    printf("4. �Ա�: %s\n", teachers[index].gender == MALE ? "��" : 
           teachers[index].gender == FEMALE ? "Ů" : "����");
    printf("5. ��������: %d-%d-%d\n", teachers[index].birth_date.year, 
           teachers[index].birth_date.month, teachers[index].birth_date.day);
    printf("6. Ժϵ: %s\n", teachers[index].department);
    printf("7. �绰: %s\n", teachers[index].phone);
    printf("8. ����: %s\n", teachers[index].email);
    printf("9. Ƹ������: %d-%d-%d\n", teachers[index].hire_date.year, 
           teachers[index].hire_date.month, teachers[index].hire_date.day);
    printf("10. ְ��: %s\n", teachers[index].title);
    printf("11. н��: %.2f\n", teachers[index].salary);
    printf("12. ״̬: %s\n", teachers[index].is_active ? "��ְ" : "��ְ");
    
    printf("\n������Ҫ�޸ĵ��ֶα�� (0-ȡ��): ");
    int field = get_int_input("");
    
    if (field == 0) {
        printf("�޸���ȡ����\n");
        return;
    }
    
    switch (field) {
        case 1:
            printf("��ʦID�����޸ġ�\n");
            break;
        case 2:
            printf("������: ");
            get_string_input("", teachers[index].name, MAX_NAME_LEN);
            break;
        case 3:
            printf("������: ");
            get_string_input("", teachers[index].password, MAX_PASSWORD_LEN);
            break;
        case 4:
            teachers[index].gender = get_gender_input("���Ա� (0-��, 1-Ů, 2-����): ");
            break;
        case 5:
            teachers[index].birth_date = get_date_input("�³������� (�� �� ��): ");
            break;
        case 6:
            printf("��Ժϵ: ");
            get_string_input("", teachers[index].department, MAX_DEPARTMENT_LEN);
            break;
        case 7:
            while (1) {
                printf("�µ绰: ");
                get_string_input("", teachers[index].phone, MAX_PHONE_LEN);
                
                if (is_valid_phone(teachers[index].phone)) {
                    break;
                } else {
                    printf("��Ч�ĵ绰���룬���������롣\n");
                }
            }
            break;
        case 8:
            while (1) {
                printf("������: ");
                get_string_input("", teachers[index].email, MAX_EMAIL_LEN);
                
                if (is_valid_email(teachers[index].email)) {
                    break;
                } else {
                    printf("��Ч�������ַ�����������롣\n");
                }
            }
            break;
        case 9:
            teachers[index].hire_date = get_date_input("��Ƹ������ (�� �� ��): ");
            break;
        case 10:
            printf("��ְ��: ");
            get_string_input("", teachers[index].title, MAX_NAME_LEN);
            break;
        case 11:
            printf("��н��: ");
            teachers[index].salary = get_float_input("");
            break;
        case 12:
            printf("��״̬ (0-��ְ, 1-��ְ): ");
            int status = get_int_input("");
            teachers[index].is_active = (status == 1);
            break;
        default:
            printf("��Ч���ֶα�š�\n");
            return;
    }
    
    printf("��ʦ��Ϣ�Ѹ��¡�\n");
}

// ɾ����ʦ
void delete_teacher() {
    printf("\n=== ɾ����ʦ ===\n");
    printf("������Ҫɾ���Ľ�ʦID: ");
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
        printf("δ�ҵ��ý�ʦ��\n");
        return;
    }
    
    // ��ʾ��ʦ��Ϣ
    printf("\n��ʦ��Ϣ:\n");
    printf("��ʦID: %s\n", teachers[index].id);
    printf("����: %s\n", teachers[index].name);
    printf("Ժϵ: %s\n", teachers[index].department);
    printf("ְ��: %s\n", teachers[index].title);
    
    // ȷ��ɾ��
    printf("\nȷ��Ҫɾ���ý�ʦ��(y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    clear_input_buffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        // �ӹ�ϣ����ɾ��
        remove_hash(&teacher_hash, id);
        
        // ����Ƿ��пγ��ɸý�ʦ����
        for (int i = 0; i < course_count; i++) {
            if (strcmp(courses[i].teacher_id, id) == 0) {
                printf("����: �ý�ʦ���ڵĿγ� %s ��Ҫ���·����ʦ��\n", courses[i].course_id);
            }
        }
        
        // ����Ƿ��а༶�ɸý�ʦ����
        for (int i = 0; i < class_count; i++) {
            if (strcmp(classes[i].teacher_id, id) == 0) {
                printf("����: �ý�ʦ����İ༶ %s ��Ҫ���·����ʦ��\n", classes[i].id);
            }
        }
        
        // ɾ����ʦ
        for (int i = index; i < teacher_count - 1; i++) {
            teachers[i] = teachers[i + 1];
        }
        teacher_count--;
        
        printf("��ʦɾ���ɹ���\n");
    } else {
        printf("ɾ����ȡ����\n");
    }
}

// �鿴��ʦ
void view_teacher() {
    printf("\n=== �鿴��ʦ ===\n");
    printf("�������ʦID: ");
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
        printf("δ�ҵ��ý�ʦ��\n");
        return;
    }
    
    printf("\n=== ��ʦ��Ϣ ===\n");
    printf("��ʦID: %s\n", teachers[index].id);
    printf("����: %s\n", teachers[index].name);
    printf("�Ա�: %s\n", teachers[index].gender == MALE ? "��" : 
           teachers[index].gender == FEMALE ? "Ů" : "����");
    printf("��������: %d-%d-%d\n", teachers[index].birth_date.year, 
           teachers[index].birth_date.month, teachers[index].birth_date.day);
    printf("Ժϵ: %s\n", teachers[index].department);
    printf("�绰: %s\n", teachers[index].phone);
    printf("����: %s\n", teachers[index].email);
    printf("Ƹ������: %d-%d-%d\n", teachers[index].hire_date.year, 
           teachers[index].hire_date.month, teachers[index].hire_date.day);
    printf("ְ��: %s\n", teachers[index].title);
    printf("н��: %.2f\n", teachers[index].salary);
    printf("״̬: %s\n", teachers[index].is_active ? "��ְ" : "��ְ");
    
    // ��ʾ���ڵĿγ�
    printf("\n=== ���ڵĿγ� ===\n");
    bool found = false;
    
    for (int i = 0; i < course_count; i++) {
        if (strcmp(courses[i].teacher_id, id) == 0) {
            printf("�γ�ID: %s\n", courses[i].course_id);
            printf("�γ�����: %s\n", courses[i].name);
            printf("ѧ��: %d\n", courses[i].credits);
            printf("ѧ��: %s\n", courses[i].semester == SPRING ? "����" : 
                   courses[i].semester == SUMMER ? "�ļ�" : 
                   courses[i].semester == FALL ? "�＾" : "����");
            printf("���: %d\n", courses[i].year);
            printf("��ǰѧ����: %d\n", courses[i].current_students);
            printf("------------------------\n");
            found = true;
        }
    }
    
    if (!found) {
        printf("�ý�ʦĿǰû�н����κογ̡�\n");
    }
    
    // ��ʾ����İ༶
    printf("\n=== ����İ༶ ===\n");
    found = false;
    
    for (int i = 0; i < class_count; i++) {
        if (strcmp(classes[i].teacher_id, id) == 0) {
            printf("�༶ID: %s\n", classes[i].id);
            printf("�༶����: %s\n", classes[i].name);
            printf("Ժϵ: %s\n", classes[i].department);
            printf("ѧ����: %d/%d\n", classes[i].student_count, classes[i].max_students);
            printf("------------------------\n");
            found = true;
        }
    }
    
    if (!found) {
        printf("�ý�ʦĿǰû�и����κΰ༶��\n");
    }
}

// �г����н�ʦ
void list_all_teachers() {
    printf("\n=== ���н�ʦ�б� ===\n");
    
    if (teacher_count == 0) {
        printf("û�н�ʦ��¼��\n");
        return;
    }
    
    printf("%-15s %-20s %-10s %-15s %-20s %-15s %-10s\n", 
           "��ʦID", "����", "�Ա�", "Ժϵ", "ְ��", "Ƹ������", "״̬");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < teacher_count; i++) {
        printf("%-15s %-20s %-10s %-15s %-20s %-15d-%d-%d %-10s\n", 
               teachers[i].id, 
               teachers[i].name, 
               teachers[i].gender == MALE ? "��" : 
               teachers[i].gender == FEMALE ? "Ů" : "����",
               teachers[i].department, 
               teachers[i].title, 
               teachers[i].hire_date.year,
               teachers[i].hire_date.month,
               teachers[i].hire_date.day,
               teachers[i].is_active ? "��ְ" : "��ְ");
    }
}

// ������ʦ
void search_teacher() {
    printf("\n=== ������ʦ ===\n");
    printf("1. ����ʦID����\n");
    printf("2. ����������\n");
    printf("3. ��Ժϵ����\n");
    printf("4. ��ְ������\n");
    printf("��ѡ��������ʽ: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1: {
            printf("�������ʦID: ");
            char id[MAX_ID_LEN];
            get_string_input("", id, MAX_ID_LEN);
            
            bool found = false;
            for (int i = 0; i < teacher_count; i++) {
                if (strcmp(teachers[i].id, id) == 0) {
                    printf("\n=== ��ʦ��Ϣ ===\n");
                    printf("��ʦID: %s\n", teachers[i].id);
                    printf("����: %s\n", teachers[i].name);
                    printf("�Ա�: %s\n", teachers[i].gender == MALE ? "��" : 
                           teachers[i].gender == FEMALE ? "Ů" : "����");
                    printf("��������: %d-%d-%d\n", teachers[i].birth_date.year, 
                           teachers[i].birth_date.month, teachers[i].birth_date.day);
                    printf("Ժϵ: %s\n", teachers[i].department);
                    printf("�绰: %s\n", teachers[i].phone);
                    printf("����: %s\n", teachers[i].email);
                    printf("Ƹ������: %d-%d-%d\n", teachers[i].hire_date.year, 
                           teachers[i].hire_date.month, teachers[i].hire_date.day);
                    printf("ְ��: %s\n", teachers[i].title);
                    printf("н��: %.2f\n", teachers[i].salary);
                    printf("״̬: %s\n", teachers[i].is_active ? "��ְ" : "��ְ");
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                printf("δ�ҵ���ʦIDΪ %s �Ľ�ʦ��\n", id);
            }
            break;
        }
        case 2: {
            printf("����������: ");
            char name[MAX_NAME_LEN];
            get_string_input("", name, MAX_NAME_LEN);
            
            bool found = false;
            printf("\n�������:\n");
            printf("%-15s %-20s %-10s %-15s %-20s %-15s %-10s\n", 
                   "��ʦID", "����", "�Ա�", "Ժϵ", "ְ��", "Ƹ������", "״̬");
            printf("--------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < teacher_count; i++) {
                if (strstr(teachers[i].name, name) != NULL) {
                    printf("%-15s %-20s %-10s %-15s %-20s %-15d-%d-%d %-10s\n", 
                           teachers[i].id, 
                           teachers[i].name, 
                           teachers[i].gender == MALE ? "��" : 
                           teachers[i].gender == FEMALE ? "Ů" : "����",
                           teachers[i].department, 
                           teachers[i].title, 
                           teachers[i].hire_date.year,
                           teachers[i].hire_date.month,
                           teachers[i].hire_date.day,
                           teachers[i].is_active ? "��ְ" : "��ְ");
                    found = true;
                }
            }
            
            if (!found) {
                printf("δ�ҵ��������� %s �Ľ�ʦ��\n", name);
            }
            break;
        }
        case 3: {
            printf("������Ժϵ: ");
            char department[MAX_DEPARTMENT_LEN];
            get_string_input("", department, MAX_DEPARTMENT_LEN);
            
            bool found = false;
            printf("\n�������:\n");
            printf("%-15s %-20s %-10s %-15s %-20s %-15s %-10s\n", 
                   "��ʦID", "����", "�Ա�", "Ժϵ", "ְ��", "Ƹ������", "״̬");
            printf("--------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < teacher_count; i++) {
                if (strstr(teachers[i].department, department) != NULL) {
                    printf("%-15s %-20s %-10s %-15s %-20s %-15d-%d-%d %-10s\n", 
                           teachers[i].id, 
                           teachers[i].name, 
                           teachers[i].gender == MALE ? "��" : 
                           teachers[i].gender == FEMALE ? "Ů" : "����",
                           teachers[i].department, 
                           teachers[i].title, 
                           teachers[i].hire_date.year,
                           teachers[i].hire_date.month,
                           teachers[i].hire_date.day,
                           teachers[i].is_active ? "��ְ" : "��ְ");
                    found = true;
                }
            }
            
            if (!found) {
                printf("δ�ҵ�ԺϵΪ %s �Ľ�ʦ��\n", department);
            }
            break;
        }
        case 4: {
            printf("������ְ��: ");
            char title[MAX_NAME_LEN];
            get_string_input("", title, MAX_NAME_LEN);
            
            bool found = false;
            printf("\n�������:\n");
            printf("%-15s %-20s %-10s %-15s %-20s %-15s %-10s\n", 
                   "��ʦID", "����", "�Ա�", "Ժϵ", "ְ��", "Ƹ������", "״̬");
            printf("--------------------------------------------------------------------------------\n");
            
            for (int i = 0; i < teacher_count; i++) {
                if (strstr(teachers[i].title, title) != NULL) {
                    printf("%-15s %-20s %-10s %-15s %-20s %-15d-%d-%d %-10s\n", 
                           teachers[i].id, 
                           teachers[i].name, 
                           teachers[i].gender == MALE ? "��" : 
                           teachers[i].gender == FEMALE ? "Ů" : "����",
                           teachers[i].department, 
                           teachers[i].title, 
                           teachers[i].hire_date.year,
                           teachers[i].hire_date.month,
                           teachers[i].hire_date.day,
                           teachers[i].is_active ? "��ְ" : "��ְ");
                    found = true;
                }
            }
            
            if (!found) {
                printf("δ�ҵ�ְ��Ϊ %s �Ľ�ʦ��\n", title);
            }
            break;
        }
        default:
            printf("��Чѡ��\n");
    }
}

// �����ʦ
void sort_teachers() {
    printf("\n=== �����ʦ ===\n");
    printf("1. ����ʦID����\n");
    printf("2. ����������\n");
    printf("3. ��н������\n");
    printf("4. ��Ƹ����������\n");
    printf("��ѡ������ʽ: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1:
            qsort(teachers, teacher_count, sizeof(Teacher), compare_teachers_by_id);
            printf("�Ѱ���ʦID����\n");
            break;
        case 2:
            qsort(teachers, teacher_count, sizeof(Teacher), compare_teachers_by_name);
            printf("�Ѱ���������\n");
            break;
        case 3:
            qsort(teachers, teacher_count, sizeof(Teacher), compare_teachers_by_salary);
            printf("�Ѱ�н������\n");
            break;
        case 4:
            // ��Ƹ������������Ҫ�Զ���ȽϺ���
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
            printf("�Ѱ�Ƹ����������\n");
            break;
        default:
            printf("��Чѡ��\n");
            return;
    }
    
    // ��ʾ�����Ľ��
    list_all_teachers();
}

// ��Ӱ༶
void add_class() {
    if (class_count >= MAX_CLASSES) {
        printf("�༶�����Ѵ����ޣ��޷���ӡ�\n");
        return;
    }
    
    printf("\n=== ��Ӱ༶ ===\n");
    
    Class new_class;
    
    // ����༶ID
    while (1) {
        printf("�༶ID: ");
        get_string_input("", new_class.id, MAX_ID_LEN);
        
        // ���༶ID�Ƿ��Ѵ���
        bool exists = false;
        for (int i = 0; i < class_count; i++) {
            if (strcmp(classes[i].id, new_class.id) == 0) {
                exists = true;
                break;
            }
        }
        
        if (exists) {
            printf("�ð༶ID�Ѵ��ڣ����������롣\n");
        } else if (!is_valid_id(new_class.id)) {
            printf("��Ч�İ༶ID��ʽ�����������롣\n");
        } else {
            break;
        }
    }
    
    // ����༶����
    printf("�༶����: ");
    get_string_input("", new_class.name, MAX_NAME_LEN);
    
    // ����Ժϵ
    printf("Ժϵ: ");
    get_string_input("", new_class.department, MAX_DEPARTMENT_LEN);
    
    // �����ʦID
    printf("��ʦID (�����򲻷���): ");
    get_string_input("", new_class.teacher_id, MAX_ID_LEN);
    
    // ��������˽�ʦID������Ƿ����
    if (strlen(new_class.teacher_id) > 0) {
        bool teacher_exists = false;
        for (int i = 0; i < teacher_count; i++) {
            if (strcmp(teachers[i].id, new_class.teacher_id) == 0 && teachers[i].is_active) {
                teacher_exists = true;
                break;
            }
        }
        
        if (!teacher_exists) {
            printf("����: ��ʦID�����ڻ��ʦ����ְ��\n");
        }
    }
    
    // �������ѧ����
    printf("���ѧ����: ");
    new_class.max_students = get_int_input("");
    
    // ��ʼ��ѧ����
    new_class.student_count = 0;
    
    // ��ӵ�����
    classes[class_count] = new_class;
    class_count++;
    
    // ��ӵ���ϣ��
    insert_hash(&class_hash, new_class.id, &classes[class_count - 1]);
    
    printf("�༶��ӳɹ���\n");
}

// �༭�༶
void edit_class() {
    printf("\n=== �༭�༶ ===\n");
    printf("������Ҫ�༭�İ༶ID: ");
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
        printf("δ�ҵ��ð༶��\n");
        return;
    }
    
    printf("\n��ǰ�༶��Ϣ:\n");
    printf("1. �༶ID: %s\n", classes[index].id);
    printf("2. �༶����: %s\n", classes[index].name);
    printf("3. Ժϵ: %s\n", classes[index].department);
    printf("4. ��ʦID: %s\n", classes[index].teacher_id);
    printf("5. ѧ����: %d\n", classes[index].student_count);
    printf("6. ���ѧ����: %d\n", classes[index].max_students);
    
    printf("\n������Ҫ�޸ĵ��ֶα�� (0-ȡ��): ");
    int field = get_int_input("");
    
    if (field == 0) {
        printf("�޸���ȡ����\n");
        return;
    }
    
    switch (field) {
        case 1:
            printf("�༶ID�����޸ġ�\n");
            break;
        case 2:
            printf("�°༶����: ");
            get_string_input("", classes[index].name, MAX_NAME_LEN);
            break;
        case 3:
            printf("��Ժϵ: ");
            get_string_input("", classes[index].department, MAX_DEPARTMENT_LEN);
            break;
        case 4:
            printf("�½�ʦID: ");
            get_string_input("", classes[index].teacher_id, MAX_ID_LEN);
            
            // ��������˽�ʦID������Ƿ����
            if (strlen(classes[index].teacher_id) > 0) {
                bool teacher_exists = false;
                for (int i = 0; i < teacher_count; i++) {
                    if (strcmp(teachers[i].id, classes[index].teacher_id) == 0 && teachers[i].is_active) {
                        teacher_exists = true;
                        break;
                    }
                }
                
                if (!teacher_exists) {
                    printf("����: ��ʦID�����ڻ��ʦ����ְ��\n");
                }
            }
            break;
        case 5:
            printf("ѧ��������ֱ���޸ġ�\n");
            break;
        case 6:
            printf("�����ѧ����: ");
            int new_max = get_int_input("");
            if (new_max < classes[index].student_count) {
                printf("����: ���ѧ��������С�ڵ�ǰѧ���� (%d)��\n", classes[index].student_count);
                printf("�Ƿ����? (y/n): ");
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
            printf("��Ч���ֶα�š�\n");
            return;
    }
    
    printf("�༶��Ϣ�Ѹ��¡�\n");
}

// ɾ���༶
void delete_class() {
    printf("\n=== ɾ���༶ ===\n");
    printf("������Ҫɾ���İ༶ID: ");
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
        printf("δ�ҵ��ð༶��\n");
        return;
    }
    
    // ��ʾ�༶��Ϣ
    printf("\n�༶��Ϣ:\n");
    printf("�༶ID: %s\n", classes[index].id);
    printf("�༶����: %s\n", classes[index].name);
    printf("Ժϵ: %s\n", classes[index].department);
    printf("��ʦID: %s\n", classes[index].teacher_id);
    printf("ѧ����: %d\n", classes[index].student_count);
    
    // ȷ��ɾ��
    printf("\nȷ��Ҫɾ���ð༶��(y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    clear_input_buffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        // �ӹ�ϣ����ɾ��
        remove_hash(&class_hash, id);
        
        // ����ѧ���İ༶ID
        for (int i = 0; i < student_count; i++) {
            if (strcmp(students[i].class_id, id) == 0) {
                strcpy(students[i].class_id, "");
            }
        }
        
        // ɾ���༶
        for (int i = index; i < class_count - 1; i++) {
            classes[i] = classes[i + 1];
        }
        class_count--;
        
        printf("�༶ɾ���ɹ���\n");
    } else {
        printf("ɾ����ȡ����\n");
    }
}

// �鿴�༶
void view_class() {
    printf("\n=== �鿴�༶ ===\n");
    printf("������༶ID: ");
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
        printf("δ�ҵ��ð༶��\n");
        return;
    }
    
    printf("\n=== �༶��Ϣ ===\n");
    printf("�༶ID: %s\n", classes[index].id);
    printf("�༶����: %s\n", classes[index].name);
    printf("Ժϵ: %s\n", classes[index].department);
    printf("��ʦID: %s\n", classes[index].teacher_id);
    printf("ѧ����: %d\n", classes[index].student_count);
    printf("���ѧ����: %d\n", classes[index].max_students);
    
    // ��ʾ�༶ѧ��
    printf("\n=== �༶ѧ�� ===\n");
    bool found = false;
    
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].class_id, id) == 0) {
            printf("ѧ��: %s\n", students[i].id);
            printf("����: %s\n", students[i].name);
            printf("�Ա�: %s\n", students[i].gender == MALE ? "��" : 
                   students[i].gender == FEMALE ? "Ů" : "����");
            printf("�꼶: %s\n", students[i].year == FRESHMAN ? "��һ" : 
                   students[i].year == SOPHOMORE ? "���" : 
                   students[i].year == JUNIOR ? "����" : 
                   students[i].year == SENIOR ? "����" : "�о���");
            printf("GPA: %.2f\n", students[i].gpa);
            printf("------------------------\n");
            found = true;
        }
    }
    
    if (!found) {
        printf("�ð༶û��ѧ����\n");
    }
}

// �г����а༶
void list_all_classes() {
    printf("\n=== ���а༶�б� ===\n");
    
    if (class_count == 0) {
        printf("û�а༶��¼��\n");
        return;
    }
    
    printf("%-15s %-20s %-15s %-15s %-15s %-15s\n", 
           "�༶ID", "�༶����", "Ժϵ", "��ʦID", "ѧ����", "���ѧ����");
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

// �����ʦ���༶
void assign_teacher_to_class() {
    printf("\n=== �����ʦ���༶ ===\n");
    printf("������༶ID: ");
    char class_id[MAX_ID_LEN];
    get_string_input("", class_id, MAX_ID_LEN);
    
    // ���Ұ༶
    int class_index = -1;
    for (int i = 0; i < class_count; i++) {
        if (strcmp(classes[i].id, class_id) == 0) {
            class_index = i;
            break;
        }
    }
    
    if (class_index == -1) {
        printf("δ�ҵ��ð༶��\n");
        return;
    }
    
    printf("��ǰ��ʦID: %s\n", classes[class_index].teacher_id);
    printf("�������µĽ�ʦID (������ȡ������): ");
    char teacher_id[MAX_ID_LEN];
    get_string_input("", teacher_id, MAX_ID_LEN);
    
    if (strlen(teacher_id) == 0) {
        // ȡ������
        strcpy(classes[class_index].teacher_id, "");
        printf("��ȡ����ʦ���䡣\n");
        return;
    }
    
    // ����ʦ�Ƿ����
    bool teacher_exists = false;
    for (int i = 0; i < teacher_count; i++) {
        if (strcmp(teachers[i].id, teacher_id) == 0 && teachers[i].is_active) {
            teacher_exists = true;
            break;
        }
    }
    
    if (!teacher_exists) {
        printf("��ʦID�����ڻ��ʦ����ְ��\n");
        return;
    }
    
    // �����ʦ
    strcpy(classes[class_index].teacher_id, teacher_id);
    printf("��ʦ����ɹ���\n");
}

// ����ѧ�����༶
void assign_student_to_class() {
    printf("\n=== ����ѧ�����༶ ===\n");
    printf("������ѧ��ѧ��: ");
    char student_id[MAX_ID_LEN];
    get_string_input("", student_id, MAX_ID_LEN);
    
    // ����ѧ��
    int student_index = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, student_id) == 0) {
            student_index = i;
            break;
        }
    }
    
    if (student_index == -1) {
        printf("δ�ҵ���ѧ����\n");
        return;
    }
    
    printf("��ǰ�༶ID: %s\n", students[student_index].class_id);
    printf("�������µİ༶ID (������ȡ������): ");
    char class_id[MAX_ID_LEN];
    get_string_input("", class_id, MAX_ID_LEN);
    
    if (strlen(class_id) == 0) {
        // ȡ������
        // ����ԭ�༶��ѧ����
        if (strlen(students[student_index].class_id) > 0) {
            for (int i = 0; i < class_count; i++) {
                if (strcmp(classes[i].id, students[student_index].class_id) == 0) {
                    classes[i].student_count--;
                    break;
                }
            }
        }
        
        strcpy(students[student_index].class_id, "");
        printf("��ȡ���༶���䡣\n");
        return;
    }
    
    // ���༶�Ƿ����
    int class_index = -1;
    for (int i = 0; i < class_count; i++) {
        if (strcmp(classes[i].id, class_id) == 0) {
            class_index = i;
            break;
        }
    }
    
    if (class_index == -1) {
        printf("�༶ID�����ڡ�\n");
        return;
    }
    
    // ���༶�Ƿ�����
    if (classes[class_index].student_count >= classes[class_index].max_students) {
        printf("�ð༶����Ա��\n");
        return;
    }
    
    // ����ԭ�༶��ѧ����
    if (strlen(students[student_index].class_id) > 0) {
        for (int i = 0; i < class_count; i++) {
            if (strcmp(classes[i].id, students[student_index].class_id) == 0) {
                classes[i].student_count--;
                break;
            }
        }
    }
    
    // ����ѧ�����°༶
    strcpy(students[student_index].class_id, class_id);
    classes[class_index].student_count++;
    
    printf("ѧ������ɹ���\n");
}

// ����ѧ������
void generate_student_report() {
    printf("\n=== ����ѧ������ ===\n");
    printf("������ѧ��ѧ�� (��������������ѧ������): ");
    char student_id[MAX_ID_LEN];
    get_string_input("", student_id, MAX_ID_LEN);
    
    if (strlen(student_id) > 0) {
        // ���ɵ���ѧ������
        int student_index = -1;
        for (int i = 0; i < student_count; i++) {
            if (strcmp(students[i].id, student_id) == 0) {
                student_index = i;
                break;
            }
        }
        
        if (student_index == -1) {
            printf("δ�ҵ���ѧ����\n");
            return;
        }
        
        printf("\n=== ѧ������: %s ===\n", students[student_index].name);
        printf("ѧ��: %s\n", students[student_index].id);
        printf("����: %s\n", students[student_index].name);
        printf("�Ա�: %s\n", students[student_index].gender == MALE ? "��" : 
               students[student_index].gender == FEMALE ? "Ů" : "����");
        printf("��������: %d-%d-%d\n", students[student_index].birth_date.year, 
               students[student_index].birth_date.month, students[student_index].birth_date.day);
        printf("Ժϵ: %s\n", students[student_index].department);
        printf("�꼶: %s\n", students[student_index].year == FRESHMAN ? "��һ" : 
               students[student_index].year == SOPHOMORE ? "���" : 
               students[student_index].year == JUNIOR ? "����" : 
               students[student_index].year == SENIOR ? "����" : "�о���");
        printf("GPA: %.2f\n", students[student_index].gpa);
        printf("��ѧ��: %d\n", students[student_index].total_credits);
        printf("��ѧ����: %d-%d-%d\n", students[student_index].admission_date.year, 
               students[student_index].admission_date.month, students[student_index].admission_date.day);
        printf("״̬: %s\n", students[student_index].is_active ? "�ڶ�" : "��ѧ/��ҵ");
        printf("�༶: %s\n", students[student_index].class_id);
        
        // ͳ��ѡ�����
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
                    
                    // ͳ�Ƶȼ�
                    if (enrollments[i].average >= 90) grade_counts[0]++;
                    else if (enrollments[i].average >= 80) grade_counts[1]++;
                    else if (enrollments[i].average >= 70) grade_counts[2]++;
                    else if (enrollments[i].average >= 60) grade_counts[3]++;
                    else grade_counts[4]++;
                }
            }
        }
        
        printf("\n=== ѡ��ͳ�� ===\n");
        printf("��ѡ����: %d\n", total_courses);
        printf("����ɿγ���: %d\n", completed_courses);
        printf("�����пγ���: %d\n", total_courses - completed_courses);
        
        if (completed_courses > 0) {
            printf("ƽ���ɼ�: %.2f\n", total_score / completed_courses);
            printf("�ȼ��ֲ�:\n");
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
            printf("û������ɿγ̵ĳɼ����ݡ�\n");
        }
    } else {
        // ��������ѧ������
        printf("\n=== ����ѧ������ ===\n");
        
        if (student_count == 0) {
            printf("û��ѧ����¼��\n");
            return;
        }
        
        // ͳ����������
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
            
            // ͳ�Ƴɼ��ȼ�
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
        
        printf("ѧ������: %d\n", total_students);
        printf("�ڶ�ѧ����: %d\n", active_students);
        printf("��ѧ/��ҵѧ����: %d\n", total_students - active_students);
        printf("ƽ��GPA: %.2f\n", total_gpa / total_students);
        printf("��ѧ��: %d\n", total_credits);
        
        if (total_grades > 0) {
            printf("\n=== �ɼ��ȼ��ֲ� ===\n");
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
            printf("\nû�гɼ����ݡ�\n");
        }
        
        // ��Ժϵͳ��
        printf("\n=== Ժϵͳ�� ===\n");
        char departments[MAX_DEPARTMENT_LEN][MAX_DEPARTMENT_LEN];
        int dept_count = 0;
        
        // �ռ�����Ժϵ
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
            printf("  ѧ����: %d\n", dept_students);
            printf("  �ڶ�ѧ����: %d\n", dept_active);
            printf("  ƽ��GPA: %.2f\n", dept_gpa / dept_students);
            printf("  ��ѧ��: %d\n", dept_credits);
        }
    }
}

// ���ɿγ̱���
void generate_course_report() {
    printf("\n=== ���ɿγ̱��� ===\n");
    printf("������γ�ID (�������������пγ̱���): ");
    char course_id[MAX_ID_LEN];
    get_string_input("", course_id, MAX_ID_LEN);
    
    if (strlen(course_id) > 0) {
        // ���ɵ����γ̱���
        int course_index = -1;
        for (int i = 0; i < course_count; i++) {
            if (strcmp(courses[i].course_id, course_id) == 0) {
                course_index = i;
                break;
            }
        }
        
        if (course_index == -1) {
            printf("δ�ҵ��ÿγ̡�\n");
            return;
        }
        
        printf("\n=== �γ̱���: %s ===\n", courses[course_index].name);
        printf("�γ�ID: %s\n", courses[course_index].course_id);
        printf("�γ�����: %s\n", courses[course_index].name);
        printf("ѧ��: %d\n", courses[course_index].credits);
        printf("Ժϵ: %s\n", courses[course_index].department);
        printf("��ʦID: %s\n", courses[course_index].teacher_id);
        printf("ѧ��: %s\n", courses[course_index].semester == SPRING ? "����" : 
               courses[course_index].semester == SUMMER ? "�ļ�" : 
               courses[course_index].semester == FALL ? "�＾" : "����");
        printf("���: %d\n", courses[course_index].year);
        printf("���ѧ����: %d\n", courses[course_index].max_students);
        printf("��ǰѧ����: %d\n", courses[course_index].current_students);
        
        // ͳ��ѧ���ɼ�
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
                    
                    // ͳ�Ƶȼ�
                    if (enrollments[i].average >= 90) grade_counts[0]++;
                    else if (enrollments[i].average >= 80) grade_counts[1]++;
                    else if (enrollments[i].average >= 70) grade_counts[2]++;
                    else if (enrollments[i].average >= 60) grade_counts[3]++;
                    else grade_counts[4]++;
                }
            }
        }
        
        printf("\n=== ѧ��ͳ�� ===\n");
        printf("ѡ��ѧ����: %d\n", total_students);
        printf("��ɿγ�ѧ����: %d\n", completed_students);
        printf("δ��ɿγ�ѧ����: %d\n", total_students - completed_students);
        
        if (completed_students > 0) {
            printf("ƽ���ɼ�: %.2f\n", total_score / completed_students);
            printf("�ȼ��ֲ�:\n");
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
            printf("û������ɿγ̵ĳɼ����ݡ�\n");
        }
    } else {
        // �������пγ̱���
        printf("\n=== ���пγ̱��� ===\n");
        
        if (course_count == 0) {
            printf("û�пγ̼�¼��\n");
            return;
        }
        
        // ͳ����������
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
                        
                        // ͳ�Ƶȼ�
                        if (enrollments[j].average >= 90) grade_counts[0]++;
                        else if (enrollments[j].average >= 80) grade_counts[1]++;
                        else if (enrollments[j].average >= 70) grade_counts[2]++;
                        else if (enrollments[j].average >= 60) grade_counts[3]++;
                        else grade_counts[4]++;
                    }
                }
            }
        }
        
        printf("�γ�����: %d\n", total_courses);
        printf("��ѡ���˴�: %d\n", total_enrollments);
        printf("��ɿγ��˴�: %d\n", total_completed);
        printf("δ��ɿγ��˴�: %d\n", total_enrollments - total_completed);
        
        if (total_completed > 0) {
            printf("ƽ���ɼ�: %.2f\n", total_score / total_completed);
        }
        
        if (total_grades > 0) {
            printf("\n=== �ɼ��ȼ��ֲ� ===\n");
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
            printf("\nû�гɼ����ݡ�\n");
        }
        
        // ��Ժϵͳ��
        printf("\n=== Ժϵͳ�� ===\n");
        char departments[MAX_DEPARTMENT_LEN][MAX_DEPARTMENT_LEN];
        int dept_count = 0;
        
        // �ռ�����Ժϵ
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
            printf("  �γ���: %d\n", dept_courses);
            printf("  ѡ���˴�: %d\n", dept_enrollments);
            printf("  ����˴�: %d\n", dept_completed);
            if (dept_completed > 0) {
                printf("  ƽ���ɼ�: %.2f\n", dept_score / dept_completed);
            }
        }
    }
}

// ���ɽ�ʦ����
void generate_teacher_report() {
    printf("\n=== ���ɽ�ʦ���� ===\n");
    printf("�������ʦID (�������������н�ʦ����): ");
    char teacher_id[MAX_ID_LEN];
    get_string_input("", teacher_id, MAX_ID_LEN);
    
    if (strlen(teacher_id) > 0) {
        // ���ɵ�����ʦ����
        int teacher_index = -1;
        for (int i = 0; i < teacher_count; i++) {
            if (strcmp(teachers[i].id, teacher_id) == 0) {
                teacher_index = i;
                break;
            }
        }
        
        if (teacher_index == -1) {
            printf("δ�ҵ��ý�ʦ��\n");
            return;
        }
        
        printf("\n=== ��ʦ����: %s ===\n", teachers[teacher_index].name);
        printf("��ʦID: %s\n", teachers[teacher_index].id);
        printf("����: %s\n", teachers[teacher_index].name);
        printf("�Ա�: %s\n", teachers[teacher_index].gender == MALE ? "��" : 
               teachers[teacher_index].gender == FEMALE ? "Ů" : "����");
        printf("Ժϵ: %s\n", teachers[teacher_index].department);
        printf("ְ��: %s\n", teachers[teacher_index].title);
        printf("Ƹ������: %d-%d-%d\n", teachers[teacher_index].hire_date.year, 
               teachers[teacher_index].hire_date.month, teachers[teacher_index].hire_date.day);
        printf("н��: %.2f\n", teachers[teacher_index].salary);
        printf("״̬: %s\n", teachers[teacher_index].is_active ? "��ְ" : "��ְ");
        
        // ͳ�ƽ��ڵĿγ�
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
                        
                        // ͳ�Ƶȼ�
                        if (enrollments[j].average >= 90) grade_counts[0]++;
                        else if (enrollments[j].average >= 80) grade_counts[1]++;
                        else if (enrollments[j].average >= 70) grade_counts[2]++;
                        else if (enrollments[j].average >= 60) grade_counts[3]++;
                        else grade_counts[4]++;
                    }
                }
            }
        }
        
        printf("\n=== ��ѧͳ�� ===\n");
        printf("���ڿγ���: %d\n", teaching_courses);
        printf("��ѧ����: %d\n", total_students);
        
        if (total_grades > 0) {
            printf("ƽ���ɼ�: %.2f\n", total_score / total_grades);
            printf("�ȼ��ֲ�:\n");
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
            printf("û�гɼ����ݡ�\n");
        }
        
        // ͳ�Ƹ���İ༶
        int managing_classes = 0;
        int class_students = 0;
        
        for (int i = 0; i < class_count; i++) {
            if (strcmp(classes[i].teacher_id, teacher_id) == 0) {
                managing_classes++;
                class_students += classes[i].student_count;
            }
        }
        
        printf("\n=== �༶���� ===\n");
        printf("����༶��: %d\n", managing_classes);
        printf("�༶ѧ����: %d\n", class_students);
    } else {
        // �������н�ʦ����
        printf("\n=== ���н�ʦ���� ===\n");
        
        if (teacher_count == 0) {
            printf("û�н�ʦ��¼��\n");
            return;
        }
        
        // ͳ����������
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
                            
                            // ͳ�Ƶȼ�
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
        
        printf("��ʦ����: %d\n", total_teachers);
        printf("��ְ��ʦ��: %d\n", active_teachers);
        printf("��ְ��ʦ��: %d\n", total_teachers - active_teachers);
        printf("ƽ��н��: %.2f\n", total_salary / total_teachers);
        printf("�ܿγ���: %d\n", teaching_courses);
        printf("��ѧ����: %d\n", total_students);
        
        if (total_grades > 0) {
            printf("ƽ���ɼ�: %.2f\n", total_score / total_grades);
        }
        
        if (total_grades > 0) {
            printf("\n=== �ɼ��ȼ��ֲ� ===\n");
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
            printf("\nû�гɼ����ݡ�\n");
        }
        
        // ��Ժϵͳ��
        printf("\n=== Ժϵͳ�� ===\n");
        char departments[MAX_DEPARTMENT_LEN][MAX_DEPARTMENT_LEN];
        int dept_count = 0;
        
        // �ռ�����Ժϵ
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
            printf("  ��ʦ��: %d\n", dept_teachers);
            printf("  ��ְ��ʦ��: %d\n", dept_active);
            printf("  ƽ��н��: %.2f\n", dept_salary / dept_teachers);
            printf("  �γ���: %d\n", dept_courses);
            printf("  ѧ����: %d\n", dept_students);
        }
    }
}

// ����Ժϵ����
void generate_department_report() {
    printf("\n=== ����Ժϵ���� ===\n");
    printf("������Ժϵ���� (��������������Ժϵ����): ");
    char department[MAX_DEPARTMENT_LEN];
    get_string_input("", department, MAX_DEPARTMENT_LEN);
    
    if (strlen(department) > 0) {
        // ���ɵ���Ժϵ����
        bool dept_exists = false;
        
        // ���Ժϵ�Ƿ����
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
            printf("δ�ҵ���Ժϵ��\n");
            return;
        }
        
        printf("\n=== Ժϵ����: %s ===\n", department);
        
        // ͳ��ѧ������
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
        
        printf("\n=== ѧ��ͳ�� ===\n");
        printf("ѧ����: %d\n", dept_students);
        printf("�ڶ�ѧ����: %d\n", dept_active_students);
        printf("��ѧ/��ҵѧ����: %d\n", dept_students - dept_active_students);
        if (dept_students > 0) {
            printf("ƽ��GPA: %.2f\n", dept_gpa / dept_students);
            printf("��ѧ��: %d\n", dept_credits);
        }
        
        // ͳ�ƽ�ʦ����
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
        
        printf("\n=== ��ʦͳ�� ===\n");
        printf("��ʦ��: %d\n", dept_teachers);
        printf("��ְ��ʦ��: %d\n", dept_active_teachers);
        printf("��ְ��ʦ��: %d\n", dept_teachers - dept_active_teachers);
        if (dept_teachers > 0) {
            printf("ƽ��н��: %.2f\n", dept_salary / dept_teachers);
        }
        
        // ͳ�ƿγ�����
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
                        
                        // ͳ�Ƶȼ�
                        if (enrollments[j].average >= 90) grade_counts[0]++;
                        else if (enrollments[j].average >= 80) grade_counts[1]++;
                        else if (enrollments[j].average >= 70) grade_counts[2]++;
                        else if (enrollments[j].average >= 60) grade_counts[3]++;
                        else grade_counts[4]++;
                    }
                }
            }
        }
        
        printf("\n=== �γ�ͳ�� ===\n");
        printf("�γ���: %d\n", dept_courses);
        printf("ѡ���˴�: %d\n", dept_enrollments);
        if (total_grades > 0) {
            printf("ƽ���ɼ�: %.2f\n", dept_score / total_grades);
            printf("�ȼ��ֲ�:\n");
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
            printf("û�гɼ����ݡ�\n");
        }
    } else {
        // ��������Ժϵ����
        printf("\n=== ����Ժϵ���� ===\n");
        
        // �ռ�����Ժϵ
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
            printf("û��Ժϵ��¼��\n");
            return;
        }
        
        // ͳ����������
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
                            
                            // ͳ�Ƶȼ�
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
        
        printf("Ժϵ��: %d\n", dept_count);
        printf("ѧ������: %d\n", total_students);
        printf("�ڶ�ѧ����: %d\n", total_active_students);
        printf("��ʦ����: %d\n", total_teachers);
        printf("��ְ��ʦ��: %d\n", total_active_teachers);
        printf("�γ�����: %d\n", total_courses);
        printf("ѡ�����˴�: %d\n", total_enrollments);
        
        if (total_students > 0) {
            printf("ƽ��GPA: %.2f\n", total_gpa / total_students);
        }
        
        if (total_teachers > 0) {
            printf("ƽ��н��: %.2f\n", total_salary / total_teachers);
        }
        
        if (total_grades > 0) {
            printf("ƽ���ɼ�: %.2f\n", total_score / total_grades);
        }
        
        if (total_grades > 0) {
            printf("\n=== �ɼ��ȼ��ֲ� ===\n");
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
            printf("\nû�гɼ����ݡ�\n");
        }
        
        // ��ʾ��Ժϵ��ϸ��Ϣ
        printf("\n=== ��Ժϵ��ϸ��Ϣ ===\n");
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
            printf("  ѧ����: %d (�ڶ�: %d)\n", dept_students, dept_active_students);
            printf("  ��ʦ��: %d (��ְ: %d)\n", dept_teachers, dept_active_teachers);
            printf("  �γ���: %d\n", dept_courses);
            printf("  ѡ���˴�: %d\n", dept_enrollments);
            if (dept_students > 0) {
                printf("  ƽ��GPA: %.2f\n", dept_gpa / dept_students);
            }
            if (dept_teachers > 0) {
                printf("  ƽ��н��: %.2f\n", dept_salary / dept_teachers);
            }
            if (dept_grades > 0) {
                printf("  ƽ���ɼ�: %.2f\n", dept_score / dept_grades);
            }
        }
    }
}

// ����ͳ����Ϣ
void generate_statistics() {
    printf("\n=== ϵͳͳ����Ϣ ===\n");
    
    // ����ͳ��
    printf("\n=== ����ͳ�� ===\n");
    printf("ѧ������: %d\n", student_count);
    printf("��ʦ����: %d\n", teacher_count);
    printf("�γ�����: %d\n", course_count);
    printf("�༶����: %d\n", class_count);
    printf("ѡ�μ�¼����: %d\n", enrollment_count);
    
    // ѧ��ͳ��
    printf("\n=== ѧ��ͳ�� ===\n");
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
    
    printf("�ڶ�ѧ����: %d\n", active_students);
    printf("��ѧ/��ҵѧ����: %d\n", inactive_students);
    if (student_count > 0) {
        printf("ƽ��GPA: %.2f\n", total_gpa / student_count);
        printf("ƽ����ѧ��: %.2f\n", (float)total_credits / student_count);
    }
    printf("�꼶�ֲ�:\n");
    printf("  ��һ: %d\n", year_counts[FRESHMAN]);
    printf("  ���: %d\n", year_counts[SOPHOMORE]);
    printf("  ����: %d\n", year_counts[JUNIOR]);
    printf("  ����: %d\n", year_counts[SENIOR]);
    printf("  �о���: %d\n", year_counts[GRADUATE]);
    
    // ��ʦͳ��
    printf("\n=== ��ʦͳ�� ===\n");
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
    
    printf("��ְ��ʦ��: %d\n", active_teachers);
    printf("��ְ��ʦ��: %d\n", inactive_teachers);
    if (teacher_count > 0) {
        printf("ƽ��н��: %.2f\n", total_salary / teacher_count);
    }
    printf("�Ա�ֲ�:\n");
    printf("  ��: %d\n", gender_counts[MALE]);
    printf("  Ů: %d\n", gender_counts[FEMALE]);
    printf("  ����: %d\n", gender_counts[OTHER]);
    
    // �γ�ͳ��
    printf("\n=== �γ�ͳ�� ===\n");
    int total_enrollments = 0;
    int semester_counts[4] = {0}; // SPRING, SUMMER, FALL, WINTER
    int credit_counts[10] = {0}; // ͳ�Ʋ�ͬѧ�ֵĿγ���
    
    for (int i = 0; i < course_count; i++) {
        total_enrollments += courses[i].current_students;
        semester_counts[courses[i].semester]++;
        
        if (courses[i].credits >= 1 && courses[i].credits <= 9) {
            credit_counts[courses[i].credits]++;
        } else {
            credit_counts[0]++; // ����ѧ��
        }
    }
    
    printf("��ѡ���˴�: %d\n", total_enrollments);
    printf("ѧ�ڷֲ�:\n");
    printf("  ����: %d\n", semester_counts[SPRING]);
    printf("  �ļ�: %d\n", semester_counts[SUMMER]);
    printf("  �＾: %d\n", semester_counts[FALL]);
    printf("  ����: %d\n", semester_counts[WINTER]);
    printf("ѧ�ֲַ�:\n");
    for (int i = 1; i <= 9; i++) {
        if (credit_counts[i] > 0) {
            printf("  %dѧ��: %d\n", i, credit_counts[i]);
        }
    }
    if (credit_counts[0] > 0) {
        printf("  ����ѧ��: %d\n", credit_counts[0]);
    }
    
    // �ɼ�ͳ��
    printf("\n=== �ɼ�ͳ�� ===\n");
    int completed_enrollments = 0;
    float total_score = 0.0;
    int grade_counts[5] = {0}; // A, B, C, D, F
    
    for (int i = 0; i < enrollment_count; i++) {
        if (enrollments[i].completed) {
            completed_enrollments++;
            total_score += enrollments[i].average;
            
            // ͳ�Ƶȼ�
            if (enrollments[i].average >= 90) grade_counts[0]++;
            else if (enrollments[i].average >= 80) grade_counts[1]++;
            else if (enrollments[i].average >= 70) grade_counts[2]++;
            else if (enrollments[i].average >= 60) grade_counts[3]++;
            else grade_counts[4]++;
        }
    }
    
    printf("��ɿγ��˴�: %d\n", completed_enrollments);
    printf("δ��ɿγ��˴�: %d\n", enrollment_count - completed_enrollments);
    if (completed_enrollments > 0) {
        printf("ƽ���ɼ�: %.2f\n", total_score / completed_enrollments);
    }
    printf("�ȼ��ֲ�:\n");
    printf("  A (90-100): %d\n", grade_counts[0]);
    printf("  B (80-89): %d\n", grade_counts[1]);
    printf("  C (70-79): %d\n", grade_counts[2]);
    printf("  D (60-69): %d\n", grade_counts[3]);
    printf("  F (0-59): %d\n", grade_counts[4]);
    
    // �༶ͳ��
    printf("\n=== �༶ͳ�� ===\n");
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
    
    printf("�༶ѧ������: %d\n", total_class_students);
    printf("�и����ʦ�İ༶��: %d\n", classes_with_teacher);
    printf("�޸����ʦ�İ༶��: %d\n", classes_without_teacher);
    
    // Ժϵͳ��
    printf("\n=== Ժϵͳ�� ===\n");
    char departments[MAX_DEPARTMENT_LEN][MAX_DEPARTMENT_LEN];
    int dept_count = 0;
    
    // �ռ�����Ժϵ
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
    
    printf("Ժϵ��: %d\n", dept_count);
    
    // ��ʾ��Ժϵ�Ļ���ͳ��
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
        
        printf("%s: ѧ�� %d, ��ʦ %d, �γ� %d\n", 
               departments[i], dept_students, dept_teachers, dept_courses);
    }
}

// ��������
void backup_data() {
    printf("\n=== �������� ===\n");
    
    char filename[100];
    printf("�����뱸���ļ��� (Ĭ��: backup.dat): ");
    get_string_input("", filename, 100);
    
    if (strlen(filename) == 0) {
        strcpy(filename, "backup.dat");
    }
    
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("�޷����������ļ���\n");
        return;
    }
    
    // д��ѧ������
    fwrite(&student_count, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), student_count, file);
    
    // д���ʦ����
    fwrite(&teacher_count, sizeof(int), 1, file);
    fwrite(teachers, sizeof(Teacher), teacher_count, file);
    
    // д��γ�����
    fwrite(&course_count, sizeof(int), 1, file);
    fwrite(courses, sizeof(Course), course_count, file);
    
    // д��༶����
    fwrite(&class_count, sizeof(int), 1, file);
    fwrite(classes, sizeof(Class), class_count, file);
    
    // д��ѡ������
    fwrite(&enrollment_count, sizeof(int), 1, file);
    fwrite(enrollments, sizeof(Enrollment), enrollment_count, file);
    
    fclose(file);
    
    printf("�����ѳɹ����ݵ� %s\n", filename);
}

// �ָ�����
void restore_data() {
    printf("\n=== �ָ����� ===\n");
    
    char filename[100];
    printf("�����뱸���ļ��� (Ĭ��: backup.dat): ");
    get_string_input("", filename, 100);
    
    if (strlen(filename) == 0) {
        strcpy(filename, "backup.dat");
    }
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("�޷��򿪱����ļ���\n");
        return;
    }
    
    // ȷ�ϻָ�
    printf("����: �ָ����ݽ����ǵ�ǰ�������ݣ��Ƿ����? (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    clear_input_buffer();
    
    if (confirm != 'y' && confirm != 'Y') {
        fclose(file);
        printf("�ָ���ȡ����\n");
        return;
    }
    
    // ��ȡѧ������
    fread(&student_count, sizeof(int), 1, file);
    fread(students, sizeof(Student), student_count, file);
    
    // ��ȡ��ʦ����
    fread(&teacher_count, sizeof(int), 1, file);
    fread(teachers, sizeof(Teacher), teacher_count, file);
    
    // ��ȡ�γ�����
    fread(&course_count, sizeof(int), 1, file);
    fread(courses, sizeof(Course), course_count, file);
    
    // ��ȡ�༶����
    fread(&class_count, sizeof(int), 1, file);
    fread(classes, sizeof(Class), class_count, file);
    
    // ��ȡѡ������
    fread(&enrollment_count, sizeof(int), 1, file);
    fread(enrollments, sizeof(Enrollment), enrollment_count, file);
    
    fclose(file);
    
    // �ؽ���ϣ��
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
    
    printf("�����ѳɹ��� %s �ָ�\n", filename);
}

// �޸�����
void change_password() {
    printf("\n=== �޸����� ===\n");
    
    char old_password[MAX_PASSWORD_LEN];
    char new_password[MAX_PASSWORD_LEN];
    char confirm_password[MAX_PASSWORD_LEN];
    
    printf("�����뵱ǰ����: ");
    get_string_input("", old_password, MAX_PASSWORD_LEN);
    
    // ��֤��ǰ����
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
        printf("��ǰ���벻��ȷ��\n");
        return;
    }
    
    printf("������������: ");
    get_string_input("", new_password, MAX_PASSWORD_LEN);
    
    printf("���ٴ�����������: ");
    get_string_input("", confirm_password, MAX_PASSWORD_LEN);
    
    if (strcmp(new_password, confirm_password) != 0) {
        printf("������������벻һ�¡�\n");
        return;
    }
    
    // ��������
    if (current_session.role == ADMIN) {
        // ����Ա����洢��students[0]
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
    
    printf("�����޸ĳɹ���\n");
}

// ϵͳ����
void system_settings() {
    printf("\n=== ϵͳ���� ===\n");
    printf("1. �޸����ѧ����\n");
    printf("2. �޸����γ���\n");
    printf("3. �޸�����ʦ��\n");
    printf("4. �޸����༶��\n");
    printf("5. �޸����ѡ�μ�¼��\n");
    printf("6. ����ϵͳ����\n");
    printf("7. ����\n");
    printf("��ѡ�����: ");
    
    int choice = get_int_input("");
    
    switch (choice) {
        case 1: {
            printf("��ǰ���ѧ����: %d\n", MAX_STUDENTS);
            printf("ע��: �޸Ĵ�ֵ��Ҫ���±������\n");
            break;
        }
        case 2: {
            printf("��ǰ���γ���: %d\n", MAX_COURSES);
            printf("ע��: �޸Ĵ�ֵ��Ҫ���±������\n");
            break;
        }
        case 3: {
            printf("��ǰ����ʦ��: %d\n", MAX_TEACHERS);
            printf("ע��: �޸Ĵ�ֵ��Ҫ���±������\n");
            break;
        }
        case 4: {
            printf("��ǰ���༶��: %d\n", MAX_CLASSES);
            printf("ע��: �޸Ĵ�ֵ��Ҫ���±������\n");
            break;
        }
        case 5: {
            printf("��ǰ���ѡ�μ�¼��: %d\n", MAX_ENROLLMENTS);
            printf("ע��: �޸Ĵ�ֵ��Ҫ���±������\n");
            break;
        }
        case 6: {
            printf("����: ����ϵͳ���ݽ�ɾ���������ݣ��Ƿ����? (y/n): ");
            char confirm;
            scanf(" %c", &confirm);
            clear_input_buffer();
            
            if (confirm == 'y' || confirm == 'Y') {
                // ������������
                student_count = 0;
                teacher_count = 0;
                course_count = 0;
                class_count = 0;
                enrollment_count = 0;
                
                // ���³�ʼ��ϵͳ
                initialize_system();
                
                printf("ϵͳ���������á�\n");
            } else {
                printf("������ȡ����\n");
            }
            break;
        }
        case 7:
            return;
        default:
            printf("��Чѡ��\n");
    }
}

// �鿴�����־
void view_audit_log() {
    printf("\n=== �����־ ===\n");
    printf("�˹�����δʵ�֡�\n");
    printf("��ʵ��ϵͳ�У��������ʾ�û���¼�������޸ĵȲ�������־��¼��\n");
}

// ������뻺����
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ��ȡ��������
int get_int_input(const char *prompt) {
    int value;
    char buffer[100];
    
    while (1) {
        if (strlen(prompt) > 0) {
            printf("%s", prompt);
        }
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("�������\n");
            continue;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            return value;
        } else {
            printf("��Ч���룬������һ��������\n");
        }
    }
}

// ��ȡ����������
float get_float_input(const char *prompt) {
    float value;
    char buffer[100];
    
    while (1) {
        if (strlen(prompt) > 0) {
            printf("%s", prompt);
        }
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("�������\n");
            continue;
        }
        
        if (sscanf(buffer, "%f", &value) == 1) {
            return value;
        } else {
            printf("��Ч���룬������һ�����֡�\n");
        }
    }
}

// ��ȡ�ַ�������
void get_string_input(const char *prompt, char *buffer, int max_len) {
    if (strlen(prompt) > 0) {
        printf("%s", prompt);
    }
    
    if (fgets(buffer, max_len, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    
    // ȥ�����з�
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    // ȥ��ǰ��ո�
    trim_whitespace(buffer);
}

// ��ȡ��������
Date get_date_input(const char *prompt) {
    Date date;
    char buffer[100];
    
    while (1) {
        if (strlen(prompt) > 0) {
            printf("%s", prompt);
        }
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("�������\n");
            continue;
        }
        
        if (sscanf(buffer, "%d %d %d", &date.day, &date.month, &date.year) == 3) {
            // ����֤����
            if (date.year < 1900 || date.year > 2100) {
                printf("�����Ч�����������롣\n");
                continue;
            }
            
            if (date.month < 1 || date.month > 12) {
                printf("�·���Ч�����������롣\n");
                continue;
            }
            
            int max_day = 31;
            if (date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11) {
                max_day = 30;
            } else if (date.month == 2) {
                // ���ж�����
                if ((date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0)) {
                    max_day = 29;
                } else {
                    max_day = 28;
                }
            }
            
            if (date.day < 1 || date.day > max_day) {
                printf("������Ч�����������롣\n");
                continue;
            }
            
            return date;
        } else {
            printf("��Ч���룬�밴�ո�ʽ���� (�� �� ��)��\n");
        }
    }
}

// ��ȡ�Ա�����
Gender get_gender_input(const char *prompt) {
    int value;
    
    while (1) {
        value = get_int_input(prompt);
        
        if (value >= 0 && value <= 2) {
            return (Gender)value;
        } else {
            printf("��Ч���룬������ 0-��, 1-Ů, 2-������\n");
        }
    }
}

// ��ȡ�꼶����
Year get_year_input(const char *prompt) {
    int value;
    
    while (1) {
        value = get_int_input(prompt);
        
        if (value >= 0 && value <= 4) {
            return (Year)value;
        } else {
            printf("��Ч���룬������ 0-��һ, 1-���, 2-����, 3-����, 4-�о�����\n");
        }
    }
}

// ��ȡ�ɼ��ȼ�����
Grade get_grade_input(const char *prompt) {
    int value;
    
    while (1) {
        value = get_int_input(prompt);
        
        if (value >= 0 && value <= 4) {
            return (Grade)value;
        } else {
            printf("��Ч���룬������ 0-A, 1-B, 2-C, 3-D, 4-F��\n");
        }
    }
}

// ��ȡѧ������
Semester get_semester_input(const char *prompt) {
    int value;
    
    while (1) {
        value = get_int_input(prompt);
        
        if (value >= 0 && value <= 3) {
            return (Semester)value;
        } else {
            printf("��Ч���룬������ 0-����, 1-�ļ�, 2-�＾, 3-������\n");
        }
    }
}

// ��ȡ�û���ɫ����
UserRole get_role_input(const char *prompt) {
    int value;
    
    while (1) {
        value = get_int_input(prompt);
        
        if (value >= 0 && value <= 3) {
            return (UserRole)value;
        } else {
            printf("��Ч���룬������ 0-����Ա, 1-��ʦ, 2-ѧ��, 3-�ÿ͡�\n");
        }
    }
}

// ��֤�����ʽ
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

// ��֤�绰�����ʽ
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

// ��֤ID��ʽ
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

// ת��ΪСд
void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + 32;
        }
    }
}

// ת��Ϊ��д
void to_upper_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 32;
        }
    }
}

// ȥ���ַ���ǰ��ո�
void trim_whitespace(char *str) {
    int i = 0;
    int j = strlen(str) - 1;
    
    // ȥ��ǰ���ո�
    while (str[i] == ' ' || str[i] == '\t') {
        i++;
    }
    
    // ȥ��β���ո�
    while (j >= i && (str[j] == ' ' || str[j] == '\t')) {
        j--;
    }
    
    // �ƶ��ַ���
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

// ��ϣ����
unsigned int hash_function(const char *key) {
    unsigned int hash = 5381;
    int c;
    
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    
    return hash % HASH_TABLE_SIZE;
}

// ��ʼ����ϣ��
void init_hash_table(HashTable *ht) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
    ht->size = 0;
}

// �����ϣ��
void insert_hash(HashTable *ht, const char *key, void *data) {
    unsigned int index = hash_function(key);
    
    HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
    if (new_node == NULL) {
        printf("�ڴ����ʧ�ܡ�\n");
        return;
    }
    
    strcpy(new_node->key, key);
    new_node->data = data;
    new_node->next = ht->table[index];
    ht->table[index] = new_node;
    ht->size++;
}

// ���ҹ�ϣ��
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

// �ӹ�ϣ����ɾ��
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

// �ͷŹ�ϣ��
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

// ��������
void save_data() {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("�޷��������ݡ�\n");
        return;
    }
    
    // д��ѧ������
    fwrite(&student_count, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), student_count, file);
    
    // д���ʦ����
    fwrite(&teacher_count, sizeof(int), 1, file);
    fwrite(teachers, sizeof(Teacher), teacher_count, file);
    
    // д��γ�����
    fwrite(&course_count, sizeof(int), 1, file);
    fwrite(courses, sizeof(Course), course_count, file);
    
    // д��༶����
    fwrite(&class_count, sizeof(int), 1, file);
    fwrite(classes, sizeof(Class), class_count, file);
    
    // д��ѡ������
    fwrite(&enrollment_count, sizeof(int), 1, file);
    fwrite(enrollments, sizeof(Enrollment), enrollment_count, file);
    
    fclose(file);
}

// ��������
void load_data() {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("δ�ҵ������ļ�����ʹ��Ĭ�����ݡ�\n");
        return;
    }
    
    // ��ȡѧ������
    fread(&student_count, sizeof(int), 1, file);
    fread(students, sizeof(Student), student_count, file);
    
    // ��ȡ��ʦ����
    fread(&teacher_count, sizeof(int), 1, file);
    fread(teachers, sizeof(Teacher), teacher_count, file);
    
    // ��ȡ�γ�����
    fread(&course_count, sizeof(int), 1, file);
    fread(courses, sizeof(Course), course_count, file);
    
    // ��ȡ�༶����
    fread(&class_count, sizeof(int), 1, file);
    fread(classes, sizeof(Class), class_count, file);
    
    // ��ȡѡ������
    fread(&enrollment_count, sizeof(int), 1, file);
    fread(enrollments, sizeof(Enrollment), enrollment_count, file);
    
    fclose(file);
    
    // �ؽ���ϣ��
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

// д���ַ������ļ�
void write_string(FILE *file, const char *str) {
    int len = strlen(str);
    fwrite(&len, sizeof(int), 1, file);
    fwrite(str, sizeof(char), len, file);
}

// ���ļ���ȡ�ַ���
void read_string(FILE *file, char *buffer, int max_len) {
    int len;
    fread(&len, sizeof(int), 1, file);
    
    if (len >= max_len) {
        len = max_len - 1;
    }
    
    fread(buffer, sizeof(char), len, file);
    buffer[len] = '\0';
    
    // ����ʣ���ַ�
    if (len < max_len - 1) {
        fseek(file, strlen(buffer) - len, SEEK_CUR);
    }
}

// �Ƚ�ѧ������
int compare_students_by_name(const void *a, const void *b) {
    const Student *student_a = (const Student *)a;
    const Student *student_b = (const Student *)b;
    return strcmp(student_a->name, student_b->name);
}

// �Ƚ�ѧ��ID
int compare_students_by_id(const void *a, const void *b) {
    const Student *student_a = (const Student *)a;
    const Student *student_b = (const Student *)b;
    return strcmp(student_a->id, student_b->id);
}

// �Ƚ�ѧ��GPA
int compare_students_by_gpa(const void *a, const void *b) {
    const Student *student_a = (const Student *)a;
    const Student *student_b = (const Student *)b;
    if (student_a->gpa < student_b->gpa) return 1;
    if (student_a->gpa > student_b->gpa) return -1;
    return 0;
}

// �ȽϿγ�����
int compare_courses_by_name(const void *a, const void *b) {
    const Course *course_a = (const Course *)a;
    const Course *course_b = (const Course *)b;
    return strcmp(course_a->name, course_b->name);
}

// �ȽϿγ�ID
int compare_courses_by_id(const void *a, const void *b) {
    const Course *course_a = (const Course *)a;
    const Course *course_b = (const Course *)b;
    return strcmp(course_a->course_id, course_b->course_id);
}

// �ȽϿγ�ѧ��
int compare_courses_by_credits(const void *a, const void *b) {
    const Course *course_a = (const Course *)a;
    const Course *course_b = (const Course *)b;
    if (course_a->credits < course_b->credits) return 1;
    if (course_a->credits > course_b->credits) return -1;
    return 0;
}

// �ȽϽ�ʦ����
int compare_teachers_by_name(const void *a, const void *b) {
    const Teacher *teacher_a = (const Teacher *)a;
    const Teacher *teacher_b = (const Teacher *)b;
    return strcmp(teacher_a->name, teacher_b->name);
}

// �ȽϽ�ʦID
int compare_teachers_by_id(const void *a, const void *b) {
    const Teacher *teacher_a = (const Teacher *)a;
    const Teacher *teacher_b = (const Teacher *)b;
    return strcmp(teacher_a->id, teacher_b->id);
}

// �ȽϽ�ʦн��
int compare_teachers_by_salary(const void *a, const void *b) {
    const Teacher *teacher_a = (const Teacher *)a;
    const Teacher *teacher_b = (const Teacher *)b;
    if (teacher_a->salary < teacher_b->salary) return 1;
    if (teacher_a->salary > teacher_b->salary) return -1;
    return 0;
}
