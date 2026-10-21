#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ADMIN_FILE "admins.txt"
#define MOD_FILE  "moderators.txt"
#define USER_FILE "users.txt"
#define ISSUE_FILE "issues.txt"
#define TEMP_FILE "temp.txt"
#define TEMP_ADMIN "temp_admin.txt"
#define SUGGESTION_FILE "suggestions.txt"
#define LINE_BUFSIZE 1024

typedef struct
{
    char username[20];
    char flat[10];
    char password[20];
} User;

typedef struct
{
    int ID;
    char type[20];
    char description[200];
    char status[50];
    char reportedBy[30];
} Issue;

void stripNewline(char *s)
{
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n')
    {
        s[len - 1] = '\0';
    }
}
void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void initAdminFile(void)
{
    FILE *f = fopen(ADMIN_FILE, "r");
    if (f)
    {
        fclose(f);
        return;
    }
    f = fopen(ADMIN_FILE, "w");
    if (!f)
    {
        fprintf(stderr, "Error: cannot create %s\n", ADMIN_FILE);
        return;
    }
    fprintf(f, "admin:admin123\n");
    fclose(f);
}
int adminLogin()
{
    char user[50], pass[50];
    printf("\n--- ADMIN LOGIN ---\n");
    printf("Enter Admin Username: ");
    if (!fgets(user, sizeof(user), stdin)) return 0;
    stripNewline(user);
    printf("Enter Password: ");
    if (!fgets(pass, sizeof(pass), stdin)) return 0;
    stripNewline(pass);
    FILE *f = fopen(ADMIN_FILE, "r");
    if (!f)
    {
        printf("No admin file (%s).\n", ADMIN_FILE);
        return 0;
    }
    char line[LINE_BUFSIZE];
    while (fgets(line, sizeof(line), f))
    {
        stripNewline(line);
        char *sep = NULL;
        for (int i = 0; line[i] != '\0'; i++)
        {
            if (line[i] == ':')
            {
                sep = &line[i];
                break;
            }
        }
        if (!sep) continue;
        *sep = '\0';
        char *u = line;
        char *p = sep + 1;
        if (strcmp(u, user) == 0 && strcmp(p, pass) == 0)
        {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    printf("\n Wrong Admin Credentials! Access Denied.\n");
    return 0;
}
void viewAllIssues()
{
    Issue i;
    FILE *fp = fopen(ISSUE_FILE, "r");

    if (fp == NULL)
    {
        printf("No issues found!\n");
        return;
    }

    printf("\n--- ALL ISSUES ---\n");

    while (fscanf(fp, "%d\n", &i.ID) == 1)
    {
        fgets(i.type, sizeof(i.type), fp);
        stripNewline(i.type);

        fgets(i.description, sizeof(i.description), fp);
        stripNewline(i.description);

        fgets(i.status, sizeof(i.status), fp);
        stripNewline(i.status);

        fgets(i.reportedBy, sizeof(i.reportedBy), fp);
        stripNewline(i.reportedBy);

        printf("\n=======================================");
        printf("\nIssue ID     : %d", i.ID);
        printf("\nIssue Type   : %s", i.type);
        printf("\nDescription  : %s", i.description);
        printf("\nStatus       : %s", i.status);
        printf("\nReported By  : %s", i.reportedBy);
        printf("\n=======================================\n");
    }

    fclose(fp);
}
int moderatorLogin()
{
    FILE *fp = fopen(MOD_FILE, "r");
    char user[20], pass[20], u[20], p[20];

    if (!fp)
    {
        printf("Moderator file missing.\n");
        return 0;
    }

    printf("\n--- MODERATOR LOGIN ---\n");

    printf("Username: ");
    fgets(user, sizeof(user), stdin);
    stripNewline(user);

    printf("Password: ");
    fgets(pass, sizeof(pass), stdin);
    stripNewline(pass);

    while (fscanf(fp, "%19s %19s", u, p) == 2)
    {
        if (strcmp(u, user) == 0 && strcmp(p, pass) == 0)
        {
            fclose(fp);
            printf("Moderator login successful.\n");
            return 1;
        }
    }

    fclose(fp);
    printf("Invalid moderator credentials.\n");
    return 0;
}
void addModerator()
{
    FILE *fp = fopen(MOD_FILE, "a");
    char user[20], pass[20];

    printf("Moderator Username: ");
    fgets(user, sizeof(user), stdin);
    stripNewline(user);

    printf("Password: ");
    fgets(pass, sizeof(pass), stdin);
    stripNewline(pass);

    fprintf(fp, "%s %s\n", user, pass);
    fclose(fp);

    printf("Moderator added.\n");
}


void updateIssueByModerator()
{
    FILE *fp=fopen("issues.txt","r");
    FILE *temp=fopen("temp.txt","w");
    Issue i;
    int id,found=0;
    if(!fp||!temp)
    {
        printf("ERROR opening file!\n");
        return;
    }
    printf("Enter Issue ID to update: ");
    scanf("%d",&id);
    getchar();
    while (fscanf(fp,"%d\n",&i.ID)==1)
    {
        fgets(i.type, sizeof(i.type),fp);
        stripNewline(i.type);
        fgets(i.description,sizeof(i.description),fp);
        stripNewline(i.description);
        fgets(i.status,sizeof(i.status),fp);
        stripNewline(i.status);
        fgets(i.reportedBy,sizeof(i.reportedBy),fp);
        stripNewline(i.reportedBy);
        if (i.ID==id)
        {
            found =1;
            printf("New Type: ");
            fgets(i.type,sizeof(i.type),stdin);
            stripNewline(i.type);
            printf("New Description: ");
            fgets(i.description,sizeof(i.description),stdin);
            stripNewline(i.description);
            printf("New Status: ");
            fgets(i.status,sizeof(i.status),stdin);
            stripNewline(i.status);
        }
        fprintf(temp,"%d\n%s\n%s\n%s\n%s\n",i.ID,i.type,i.description,i.status,i.reportedBy);
    }
    fclose(fp);
    fclose(temp);
    remove("issues.txt");
    rename("temp.txt",ISSUE_FILE);
    if (found)
        printf("Issue updated!\n");
    else
        printf("Issue ID %d not found!\n",id);
}
void searchIssueByIDModerator()
{
    FILE *fp=fopen("issues.txt","r");
    if(!fp)
    {
        printf("No issues file found.\n");
        return;
    }
    int id,found=0;
    Issue i;
    printf("Enter Issue ID to search: ");
    scanf("%d",&id);
    getchar();
    while (fscanf(fp,"%d\n",&i.ID)== 1)
    {
        fgets(i.type, sizeof(i.type),fp);
        stripNewline(i.type);
        fgets(i.description, sizeof(i.description),fp);
        stripNewline(i.description);
        fgets(i.status, sizeof(i.status),fp);
        stripNewline(i.status);
        fgets(i.reportedBy, sizeof(i.reportedBy),fp);
        stripNewline(i.reportedBy);
        if (i.ID==id)
        {
            found=1;
            printf("\nID: %d\nType: %s\nDescription: %s\nStatus: %s\nReported By: %s\n",i.ID,i.type,i.description,i.status,i.reportedBy);
            break;
        }
    }
    fclose(fp);
    if(!found)
    {
        printf("Issue with ID %d not found.\n",id);
    }
}
void searchIssueByID()
{
    FILE *fp = fopen(ISSUE_FILE, "r");
    Issue i;
    int searchID;
    int found = 0;

    if (fp == NULL)
    {
        printf("No issues found.\n");
        return;
    }
    printf("Enter issue ID to search: ");
    scanf("%d",&searchID);
    clearInputBuffer();
    while (fscanf(fp, "%d\n", &i.ID) == 1)
    {
        fgets(i.type, sizeof(i.type), fp);
        stripNewline(i.type);

        fgets(i.description, sizeof(i.description), fp);
        stripNewline(i.description);

        fgets(i.status, sizeof(i.status), fp);
        stripNewline(i.status);

        fgets(i.reportedBy, sizeof(i.reportedBy), fp);
        stripNewline(i.reportedBy);

        if(i.ID==searchID)
        {
            found=1;
            printf("\n======ISSUE FOUND========");
            printf("\nIssue ID     : %d", i.ID);
            printf("\nIssue Type   : %s", i.type);
            printf("\nDescription  : %s", i.description);
            printf("\nStatus       : %s", i.status);
            printf("\nReported By  : %s", i.reportedBy);
            printf("\n=======================================\n");
            break;
        }
    }
    if(found==0)
    {
        printf("Issue not found with ID: %d",searchID);
    }

    fclose(fp);
}
void issuePercentage()
{
    FILE *fp = fopen(ISSUE_FILE, "r");
    Issue i;
    int personal=0,water=0,electrical=0,gas=0,chaos=0,security=0,others=0,total=0;

    if (fp == NULL)
    {
        printf("No issues found!\n");
        return;
    }
    while (fscanf(fp, "%d\n", &i.ID) == 1)
    {
        fgets(i.type, sizeof(i.type), fp);
        stripNewline(i.type);

        fgets(i.description, sizeof(i.description), fp);
        stripNewline(i.description);

        fgets(i.status, sizeof(i.status), fp);
        stripNewline(i.status);

        fgets(i.reportedBy, sizeof(i.reportedBy), fp);
        stripNewline(i.reportedBy);

        total++;

        if(strcmp(i.type,"personal")==0)
            personal++;
        else if(strcmp(i.type,"water")==0)
            water++;
        else if(strcmp(i.type,"electrical")==0)
            electrical++;
        else if(strcmp(i.type,"gas")==0)
            gas++;
        else if(strcmp(i.type,"chaos")==0)
            chaos++;
        else if(strcmp(i.type,"security")==0)
            security++;
        else if(strcmp(i.type,"others")==0)
            others++;
    }
    fclose(fp);

    if(total==0)
    {
        printf("No issues available.\n");
    }

    printf("\n--- ISSUE PERCENTAGE REPORT ---\n");
    printf("Personal    :%.2f%%\n",(personal*100.0)/total);
    printf("Water       :%.2f%%\n",(water*100.0)/total);
    printf("Electrical  :%.2f%%\n",(electrical*100.0)/total);
    printf("Gas         :%.2f%%\n",(gas*100.0)/total);
    printf("Chaos       :%.2f%%\n",(chaos*100.0)/total);
    printf("Security    :%.2f%%\n",(security*100.0)/total);
    printf("Others      :%.2f%%\n",(others*100.0)/total);
}
void moderatorPanel()
{
    int ch;
    do
    {
        printf("\n--- MODERATOR PANEL ---");
        printf("\n1. View All Issues");
        printf("\n2. Update Issue Status");
        printf("\n3. Search Issue by ID");
        printf("\n4. Return to Main Menu");
        printf("\nEnter choice: ");
        if (scanf("%d",&ch) != 1)
        {
            clearInputBuffer();
            printf("Invalid input.\n");
            continue;
        }
        clearInputBuffer();
        switch(ch)
        {
        case 1:
            viewAllIssues();
            break;
        case 2:
            updateIssueByModerator();
            break;
        case 3:
            searchIssueByIDModerator();
            break;
        default:
            if(ch!= 4)
                printf("Invalid choice.\n");
        }

    }
    while(ch!= 4);
}

void updateIssue()
{
    FILE *fp=fopen("issues.txt","r");
    FILE *temp=fopen("temp.txt","w");
    Issue i;
    int id,found=0;
    if(!fp||!temp)
    {
        printf("ERROR opening file!\n");
        return;
    }
    printf("Enter Issue ID to update: ");
    scanf("%d",&id);
    getchar();
    while (fscanf(fp,"%d\n",&i.ID)==1)
    {
        fgets(i.type, sizeof(i.type),fp);
        stripNewline(i.type);
        fgets(i.description,sizeof(i.description),fp);
        stripNewline(i.description);
        fgets(i.status,sizeof(i.status),fp);
        stripNewline(i.status);
        fgets(i.reportedBy,sizeof(i.reportedBy),fp);
        stripNewline(i.reportedBy);
        if (i.ID==id)
        {
            found =1;
            printf("New Type: ");
            fgets(i.type,sizeof(i.type),stdin);
            stripNewline(i.type);
            printf("New Description: ");
            fgets(i.description,sizeof(i.description),stdin);
            stripNewline(i.description);
            printf("New Status: ");
            fgets(i.status,sizeof(i.status),stdin);
            stripNewline(i.status);
        }
        fprintf(temp,"%d\n%s\n%s\n%s\n%s\n",i.ID,i.type,i.description,i.status,i.reportedBy);
    }
    fclose(fp);
    fclose(temp);
    remove("issues.txt");
    rename("temp.txt",ISSUE_FILE);
    if (found)
        printf("Issue updated!\n");
    else
        printf("Issue ID %d not found!\n",id);
}
void viewAdmins()
{
    FILE *f = fopen(ADMIN_FILE, "r");
    if (!f)
    {
        printf("No admin file (%s).\n", ADMIN_FILE);
        return;
    }
    char line[LINE_BUFSIZE];
    printf("\nAdmins (from %s):\n", ADMIN_FILE);
    while (fgets(line, sizeof(line), f))
    {
        stripNewline(line);
        char *sep = NULL;
        for (int i = 0; line[i] != '\0'; i++)
        {
            if (line[i] == ':')
            {
                sep = &line[i];
                break;
            }
        }
        if (!sep) continue;
        *sep = '\0';
        printf("Username: %s\n", line);
    }
    fclose(f);
}
int adminExists(const char *username)
{
    FILE *f = fopen(ADMIN_FILE, "r");
    if (!f) return 0;
    char line[LINE_BUFSIZE];
    while (fgets(line, sizeof(line), f))
    {
        stripNewline(line);
        char *sep = NULL;
        for (int i = 0; line[i] != '\0'; i++)
        {
            if (line[i] == ':')
            {
                sep = &line[i];
                break;
            }
        }
        if (!sep) continue;
        *sep = '\0';
        if (strcmp(line, username) == 0)
        {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}
void addAdmin()
{
    char username[50], password[50];
    printf("New admin username: ");
    if (!fgets(username, sizeof(username), stdin))
    {
        printf("Read failed.\n");
        return;
    }
    stripNewline(username);
    if (username[0] == '\0')
    {
        printf("Username cannot be empty.\n");
        return;
    }
    if (adminExists(username))
    {
        printf("Admin already exists.\n");
        return;
    }
    printf("Password: ");
    if (!fgets(password, sizeof(password), stdin))
    {
        printf("Read failed.\n");
        return;
    }
    stripNewline(password);
    FILE *f = fopen(ADMIN_FILE, "a");
    if (!f)
    {
        printf("Cannot open %s for append.\n", ADMIN_FILE);
        return;
    }
    fprintf(f, "%s:%s\n", username, password);
    fclose(f);
    printf("Admin '%s' added.\n", username);
}
void viewSuggestions()
{
    FILE *fp;
    fp=fopen(SUGGESTION_FILE,"r");
    char user[30],text[200];

    if(fp==NULL)
    {
        printf("No suggestions found.\n");
        return;
    }

    printf("\n--- SUGGESTIONS ---\n");
    while(fgets(user,sizeof(user),fp))
    {
        stripNewline(user);
        fgets(text,sizeof(text),fp);
        stripNewline(text);

        printf("\nUser      :%s",user);
        printf("\nSuggestion:%s\n",text);
    }

    fclose(fp);


}
void removeAdmin()
{
    char name[50],line[LINE_BUFSIZE],user[50];
    int i,removed =0;
    printf("Enter admin username to remove: ");
    fgets(name,sizeof(name),stdin);
    stripNewline(name);
    FILE *fp =fopen("admins.txt","r");
    FILE *tp =fopen("temp_admin.txt","w");
    if (!fp||!tp)
    {
        printf("File error.\n");
        return;
    }

    while(fgets(line,sizeof(line),fp))
    {
        i=0;
        while(line[i]!=':' && line[i]!='\n' && line[i]!='\0')
        {
            user[i]=line[i];
            i++;
        }
        user[i]='\0';
        if(strcmp(user,name)==0)
        {
            removed=1;
        }
        else
        {
            fputs(line,tp);
        }
    }
    fclose(fp);
    fclose(tp);
    if(!removed)
    {
        remove("temp_admin.txt");
        printf("Admin not found.\n");
        return;
    }
    remove("admins.txt");
    rename("temp_admin.txt","admins.txt");
    printf("Admin removed successfully.\n");
}
void removeUser()
{
    char username[50],line[LINE_BUFSIZE],user[50];
    int i,removed =0;
    printf("Enter username to remove: ");
    fgets(username,sizeof(username),stdin);
    stripNewline(username);
    FILE *fp =fopen("users.txt","r");
    FILE *tp =fopen("temp_user.txt","w");
    if (!fp||!tp)
    {
        printf("File error.\n");
        return;
    }
    while(fgets(line,sizeof(line),fp))
    {
        i=0;
        while(line[i]!=':' && line[i]!='\0' && line[i]!='\n') user[i]=line[i++];
        user[i]='\0';
        if (strcmp(user,username)==0)
        {
            removed=1;
        }
        else
        {
            fputs(line,tp);
        }
    }
    fclose(fp);
    fclose(tp);
    if(!removed)
    {
        remove("temp_user.txt");
        printf("User not found.\n");
        return;
    }
    remove("users.txt");
    rename("temp_user.txt","users.txt");
    printf("User removed successfully.\n");
}
void removeModerator()
{
    char username[50],line[LINE_BUFSIZE],user[50];
    int i,removed =0;
    printf("Enter moderator username to remove: ");
    fgets(username,sizeof(username),stdin);
    stripNewline(username);
    FILE *fp=fopen("moderators.txt","r");
    FILE *tp=fopen("temp_mod.txt","w");
    if (!fp||!tp)
    {
        printf("File error.\n");
        return;
    }
    while(fgets(line,sizeof(line),fp))
    {
        i=0;
        while(line[i]!=':' && line[i]!='\0' && line[i]!='\n') user[i]=line[i++];
        user[i] ='\0';
        if(strcmp(user,username)== 0)
        {
            removed=1;
        }
        else
        {
            fputs(line,tp);
        }
    }
    fclose(fp);
    fclose(tp);
    if(!removed)
    {
        remove("temp_mod.txt");
        printf("Moderator not found.\n");
        return;
    }
    remove("mods.txt");
    rename("temp_mod.txt","mods.txt");
    printf("Moderator removed successfully.\n");
}
void viewModerators()
{
    char line[LINE_BUFSIZE],name[50];
    int i;
    FILE *fp =fopen("moderators.txt","r");
    if (!fp)
    {
        printf("No moderators found.\n");
        return;
    }
    printf("\nModerators:\n");
    while (fgets(line,sizeof(line),fp))
    {
        i=0;
        while(line[i]!=':' && line[i]!='\n' && line[i]!='\0')
        {
            name[i]=line[i++];
        }
        name[i] = '\0';
        printf("%s\n",name);
    }
    fclose(fp);
}
void viewUsers()
{
    char line[LINE_BUFSIZE],name[50];
    int i;
    FILE *fp=fopen("users.txt","r");
    if (!fp)
    {
        printf("No users found.\n");
        return;
    }
    printf("\nUsers:\n");
    while (fgets(line,sizeof(line),fp))
    {
        i=0;
        while (line[i]!=':' && line[i]!='\n' && line[i]!='\0')
        {
            name[i]=line[i++];
        }
        name[i]='\0';
        printf("%s\n",name);
    }
    fclose(fp);
}
void changeUserPassword(char username[])
{
    char newpass[50];
    char line[LINE_BUFSIZE],user[50],pass[50];
    int i,j,found = 0;
    printf("Enter new password: ");
    fgets(newpass,sizeof(newpass),stdin);
    stripNewline(newpass);
    FILE *fp= fopen("users.txt","r");
    if (!fp)
    {
        printf("File not found.\n");
        return;
    }
    FILE *tp=fopen("temp_user.txt","w");
    if(!tp)
    {
        fclose(fp);
        printf("File error.\n");
        return;
    }
    while(fgets(line,sizeof(line),fp))
    {
        i=0;
        while (line[i]!=':' && line[i]!='\0' && line[i]!='\n')
        {
            user[i]=line[i++];
        }
        user[i]='\0';
        if(line[i]==':')
            i++;
        j = 0;
        while(line[i]!='\n' && line[i]!='\0')
        {
            pass[j++] =line[i++];
        }
        pass[j]='\0';
        if(strcmp(user,username)==0)
        {
            fprintf(tp,"%s:%s\n",user,newpass);
            found=1;
        }
        else
        {
            fprintf(tp,"%s:%s\n",user,pass);
        }
    }
    fclose(fp);
    fclose(tp);
    if(!found)
    {
        remove("temp_user.txt");
        printf("User not found.\n");
        return;
    }
    remove("users.txt");
    rename("temp_user.txt","users.txt");
    printf("Password changed successfully.\n");
}
void changeAdminPassword()
{
    char username[50],newpass[50];
    char line[LINE_BUFSIZE],user[50],pass[50];
    int i,j,found=0;
    printf("Enter admin username: ");
    fgets(username,sizeof(username),stdin);
    stripNewline(username);
    printf("Enter new password: ");
    fgets(newpass,sizeof(newpass),stdin);
    stripNewline(newpass);
    FILE *fp=fopen("admins.txt","r");
    if(!fp)
    {
        printf("File not found.\n");
        return;
    }
    FILE *tp=fopen("temp_admin.txt","w");
    if(!tp)
    {
        fclose(fp);
        printf("File error.\n");
        return;
    }
    while(fgets(line,sizeof(line),fp))
    {
        i=0;
        while(line[i]!=':' && line[i]!='\0' && line[i]!='\n')
        {
            user[i]=line[i++];
        }
        user[i]='\0';
        if(line[i]==':')
            i++;
        j = 0;
        while(line[i]!='\n' && line[i]!='\0')
        {
            pass[j++]=line[i++];
        }
        pass[j] ='\0';
        if(strcmp(user, username)==0)
        {
            fprintf(tp,"%s:%s\n",user,newpass);
            found=1;
        }
        else
        {
            fprintf(tp,"%s:%s\n",user,pass);
        }
    }
    fclose(fp);
    fclose(tp);
    if(!found)
    {
        remove("temp_admin.txt");
        printf("Admin not found.\n");
        return;
    }
    remove("admins.txt");
    rename("temp_admin.txt", "admins.txt");
    printf("Password changed successfully.\n");
}
void changeModeratorPassword()
{
    char name[50], newpass[50];
    char line[LINE_BUFSIZE],user[50],pass[50];
    int i,j,found = 0;
    printf("Enter moderator username: ");
    fgets(name,sizeof(name),stdin);
    stripNewline(name);
    printf("Enter new password: ");
    fgets(newpass,sizeof(newpass),stdin);
    stripNewline(newpass);
    FILE *fp=fopen("moderators.txt","r");
    if(!fp)
    {
        printf("File not found.\n");
        return;
    }
    FILE *tp=fopen("temp_moderators.txt","w");
    if(!tp)
    {
        fclose(fp);
        printf("File error.\n");
        return;
    }
    while(fgets(line,sizeof(line),fp))
    {
        i=0;
        while(line[i]!=':' && line[i]!='\0' && line[i]!='\n')
        {
            user[i]=line[i++];
        }
        user[i]='\0';
        if(line[i]==':')
            i++;
        j=0;
        while(line[i]!='\n' && line[i]!='\0')
        {
            pass[j++]=line[i++];
        }
        pass[j]='\0';
        if(strcmp(user,name)==0)
        {
            fprintf(tp,"%s:%s\n",user,newpass);
            found=1;
        }
        else
        {
            fprintf(tp,"%s:%s\n",user,pass);
        }
    }
    fclose(fp);
    fclose(tp);
    if(!found)
    {
        remove("temp_moderators.txt");
        printf("Moderator not found.\n");
        return;
    }
    remove("moderators.txt");
    rename("temp_moderators.txt","moderators.txt");
    printf("Moderator password changed successfully.\n");
}
void promoteUserToModerator()
{
    char username[50],line[LINE_BUFSIZE],user[50],password[50];
    int i,j,found=0;
    printf("Enter username to promote: ");
    fgets(username,sizeof(username),stdin);
    stripNewline(username);
    FILE *fp=fopen("users.txt","r");
    if (!fp)
    {
        printf("Users file not found.\n");
        return;
    }
    FILE *fpMod=fopen("moderators.txt","a");
    if(!fpMod)
    {
        fclose(fp);
        printf("Moderator file error.\n");
        return;
    }
    while(fgets(line,sizeof(line),fp))
    {
        i=0;
        while(line[i]!=':' && line[i]!='\0' && line[i]!='\n')
        {
            user[i]=line[i++];
        }
        user[i]='\0';
        if(line[i]==':')
            i++;
        j=0;
        while(line[i]!='\n' && line[i]!='\0')
        {
            password[j++]=line[i++];
        }
        password[j]='\0';
        if(strcmp(user,username)==0)
        {
            fprintf(fpMod,"%s:%s\n",user,password);
            found=1;
            break;
        }
    }
    fclose(fp);
    fclose(fpMod);
    if(found)
        printf("User '%s' promoted to moderator.\n",username);
    else
        printf("User not found.\n");
}
void demoteModeratorToUser()
{
    char username[50],line[LINE_BUFSIZE],user[50],password[50];
    int i,j,found=0;
    printf("Enter moderator username to demote: ");
    fgets(username,sizeof(username),stdin);
    stripNewline(username);
    FILE *fp=fopen("moderators.txt","r");
    if(!fp)
    {
        printf("Moderators file not found.\n");
        return;
    }
    FILE *fpTemp=fopen("temp_moderators.txt","w");
    if(!fpTemp)
    {
        fclose(fp);
        printf("Temporary file error.\n");
        return;
    }
    while(fgets(line,sizeof(line),fp))
    {
        i=0;
        while(line[i]!=':' && line[i]!='\0' && line[i]!='\n')
        {
            user[i]=line[i++];
        }
        user[i]='\0';
        if(line[i]==':')
            i++;
        j=0;
        while(line[i]!='\n' && line[i]!='\0')
        {
            password[j++]=line[i++];
        }
        password[j]='\0';
        if(strcmp(user, username)==0)
        {
            FILE *fpUser=fopen("users.txt","a");
            if(fpUser)
            {
                fprintf(fpUser,"%s:%s\n",user,password);
                fclose(fpUser);
            }
            found=1;
        }
        else
        {
            fputs(line,fpTemp);
        }
    }
    fclose(fp);
    fclose(fpTemp);
    if(!found)
    {
        remove("temp_moderators.txt");
        printf("Moderator not found.\n");
        return;
    }
    remove("moderators.txt");
    rename("temp_moderators.txt","moderators.txt");
    printf("Moderator '%s' demoted to user.\n",username);
}
void adminPanel()
{
    int ch;
    do
    {
        printf("\n--- ADMIN PANEL ---");
        printf("\n1.  Update Issue Status");
        printf("\n2.  View Admins");
        printf("\n3.  Add Admin");
        printf("\n4.  Remove Admin");
        printf("\n5.  Add Moderator");
        printf("\n6.  Remove User");
        printf("\n7.  Remove Moderator");
        printf("\n8.  View Suggestions");
        printf("\n9.  View issue percentage");
        printf("\n10. Search Issue By ID.");
        printf("\n11.  View Moderators");
        printf("\n12. View Users");
        printf("\n13. Change User Password");
        printf("\n14. Change Admin Password");
        printf("\n15. Change Moderator Password");
        printf("\n16. Promote User to Moderator");
        printf("\n17. Demote Moderator to User");
        printf("\n18. Logout");
        printf("\nEnter choice: ");
        if(scanf("%d", &ch)!=1)
        {
            clearInputBuffer();
            printf("Invalid input.\n");
            continue;
        }
        clearInputBuffer();
        switch(ch)
        {
        case 1:
            updateIssue();
            break;
        case 2:
            viewAdmins();
            break;
        case 3:
            addAdmin();
            break;
        case 4:
            removeAdmin();
            break;
        case 5:
            addModerator();
            break;
        case 6:
            removeUser();
            break;
        case 7:
            removeModerator();
            break;
        case 8:
            viewSuggestions();
            break;
        case 9:
            issuePercentage();
            break;
        case 10:
            searchIssueByID();
            break;
        case 11:
            viewModerators();
            break;
        case 12:
            viewUsers();
            break;
        case 13:
        {
            char username[50];
            printf("Enter username to change password: ");
            fgets(username,sizeof(username),stdin);
            stripNewline(username);
            changeUserPassword(username);
            break;
        }
        case 14:
            changeAdminPassword();
            break;
        case 15:
            changeModeratorPassword();
            break;
        case 16:
            promoteUserToModerator();
            break;
        case 17:
            demoteModeratorToUser();
            break;
        case 18:
            break;
        default:
            printf("Invalid choice.\n");
        }

    }
    while(ch!=18);
}

void SignUp(void)
{
    FILE *fp;
    User u;
    char username[20];
    char nflat[20];
    char pass[20];
    printf("Enter New Username: ");
    fgets(username,sizeof(username),stdin);
    stripNewline(username);

    printf("Enter New Flat No.: ");
    fgets(nflat,sizeof(nflat),stdin);
    stripNewline(nflat);

    printf("Enter new password(max 20 letters): ");
    fgets(pass,sizeof(pass),stdin);
    stripNewline(pass);

    fp=fopen(USER_FILE,"r");
    if(fp)
    {
        User temp;
        while(fscanf(fp,"%s %s",temp.username,temp.flat)==2)
        {
            if((strcmp(temp.username, username) == 0) && (strcmp(temp.flat, nflat) == 0))
            {
                printf("Username and flat no. is already taken.\n");
                fclose(fp);
                return;
            }
        }
        fclose(fp);
    }
    strcpy(u.username,username);
    strcpy(u.flat,nflat);
    strcpy(u.password,pass);

    fp=fopen(USER_FILE,"a");
    fprintf(fp,"%s %s %s\n",u.username,u.flat,u.password);
    fclose(fp);

    printf("Signed Up Successfully!!!");

}
int login(char loggedUser[])
{

    FILE *fp= fopen(USER_FILE,"r");
    if(!fp)
    {
        printf("No user found.\n");
        return 0;
    }
    char line[LINE_BUFSIZE];

    char enterName[20],enterFlat[20],enterPass[20];

    printf("Username: ");
    fgets(enterName,sizeof(enterName),stdin);
    stripNewline(enterName);

    printf("Flat NO: ");
    fgets(enterFlat,sizeof(enterFlat),stdin);
    stripNewline(enterFlat);

    printf("Password: ");
    fgets(enterPass,sizeof(enterPass),stdin);
    stripNewline(enterPass);

    User tempuser;
    int Successful=0;

    while(fscanf(fp,"%19s %19s %19s",tempuser.username,tempuser.flat,tempuser.password)==3 )
    {
        if (strcmp(tempuser.username, enterName) == 0 &&
                strcmp(tempuser.flat, enterFlat) == 0 &&
                strcmp(tempuser.password, enterPass) == 0)
        {
            strcpy(loggedUser,tempuser.username);
            printf("Login Successful!!!\n");
            Successful=1;
            break;
        }
    }


    fclose(fp);

    if(Successful)
    {
        return 1;
    }
    else
    {
        printf("Invalid.\n");
        return 0;
    }


}

void reportIssue(const char *username)
{
    FILE *fp;
    Issue issue,temp;
    int max_id=0;

    fp = fopen(ISSUE_FILE, "r");
    if (fp != NULL)
    {
        while (1)
        {
            if (fscanf(fp, "%d\n", &temp.ID) != 1)
                break;

            fgets(temp.type, sizeof(temp.type), fp);
            stripNewline(temp.type);

            fgets(temp.description, sizeof(temp.description), fp);
            stripNewline(temp.description);

            fgets(temp.status, sizeof(temp.status), fp);
            stripNewline(temp.status);

            fgets(temp.reportedBy, sizeof(temp.reportedBy), fp);
            stripNewline(temp.reportedBy);

            if (temp.ID > max_id)
                max_id = temp.ID;
        }
        fclose(fp);
    }

    issue.ID=max_id+1;

    printf("Issue type(Personal/Water/Electrical/Gas/Chaos/Security/Others): ");
    fgets(issue.type,sizeof(issue.type),stdin);
    stripNewline(issue.type);

    printf("Description: ");
    fgets(issue.description,sizeof(issue.description),stdin);
    stripNewline(issue.description);

    strcpy(issue.status,"Pending");
    strcpy(issue.reportedBy,username);

    fp=fopen(ISSUE_FILE,"a");
    if(fp==NULL)
    {
        printf("ERROR!");
        return;
    }
    fprintf(fp, "%d\n", issue.ID);
    fprintf(fp, "%s\n", issue.type);
    fprintf(fp, "%s\n", issue.description);
    fprintf(fp, "%s\n", issue.status);
    fprintf(fp, "%s\n", issue.reportedBy);


    fclose(fp);
    printf("Issue reported successfully with ID %d",issue.ID);

}


void addSuggestion(const char *username)
{
    FILE *fp;
    char suggestion[200];
    printf("Enter your suggestion: ");
    fgets(suggestion,sizeof(suggestion),stdin);
    stripNewline(suggestion);
    fp=fopen(SUGGESTION_FILE,"a");

    if(fp==NULL)
    {
        printf("Error saving suggestion.\n");
        return;
    }

    fprintf(fp,"%s\n",username);
    fprintf(fp,"%s\n",suggestion);

    fclose(fp);

    printf("Suggestion submitted successfully.\n");
}

void userPanel(const char *username)
{
    int ch;
    do
    {
        printf("\n---USER PANEL---");
        printf("\n1.Report issues.");
        printf("\n2.View issues.");
        printf("\n3.View issue percentage.");
        printf("\n4.Give suggestion.");
        printf("\n5.Logout");
        printf("\n.Enter choice: ");

        if(scanf("%d",&ch)!=1)
        {
            clearInputBuffer();
            printf("Invalid input.\n");
            continue;
        }
        clearInputBuffer();
        switch (ch)
        {
        case 1:
            reportIssue(username);
            break;
        case 2:
            viewAllIssues();
            break;
        case 3:
            issuePercentage();
            break;
        case 4:
            addSuggestion(username);
            break;
        case 5:
            break;
        default:
            printf("Invalid choice.\n");
        }
    }
    while(ch!=5);
}

int main()
{
    initAdminFile();
    int choice;
    char user[30];
    printf("Enter choice: ");
    while (1)
    {
        printf("\n1.Sign Up\n2.User Login\n3.Admin Login\n4.Moderator Login\n5.Exit\nChoice: ");
        scanf("%d", &choice);
        clearInputBuffer();


        if (choice == 1) SignUp();
        else if (choice == 2 && login(user))userPanel(user);
        else if (choice == 3 && adminLogin()) adminPanel();
        else if (choice == 4 && moderatorLogin()) moderatorPanel();
        else if (choice == 5) exit(0);
        else printf("Invalid or failed login.\n");
    }


}

