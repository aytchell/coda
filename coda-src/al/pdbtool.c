/* BLURB gpl

                           Coda File System
                              Release 5

          Copyright (c) 1987-1999 Carnegie Mellon University
                  Additional copyrights listed below

This  code  is  distributed "AS IS" without warranty of any kind under
the terms of the GNU General Public Licence Version 2, as shown in the
file  LICENSE.  The  technical and financial  contributors to Coda are
listed in the file CREDITS.

                        Additional copyrights

   Copyright (C) 1998  John-Anthony Owens, Samuel Ieong, Rudi Seitz

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

*/


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "pdb.h"
#include <parser.h>
#include <string.h>
#include <coda_assert.h>

/* Check if correct number of arguments,Too Few=1,Too Many=2,Just Right=0 */
int check_args_num(int argc,int n){
	if(argc<n){
		printf("Too few arguments!\n");
		return 1;
	}
	else if(argc > n){
		printf("Too many arguments!\n");
		return 2;
	}
	else return 0;
}

/* LOOKUP BY ID */
void tool_byId(int argc,char *argv[]){
	PDB_profile sample;
	PDB_HANDLE h;
	long arg1;
	if(check_args_num(argc,2)){
		printf("Usage: i idnum\nidnum\t\tnumber of user/group\n");
		return;
	}
	arg1 = atol(argv[1]);
	if(arg1 == 0){
		printf("Give numerical value.\n");
		return;
	}
	h = PDB_db_open(O_RDONLY);
	PDB_readProfile(h, arg1, &sample);
	PDB_printProfile(stdout, &sample);
	PDB_freeProfile(&sample);
	PDB_db_close(h);
}

/* LOOKUP BY NAME */
void tool_byName(int argc,char *argv[]){
	int32_t id;
	PDB_profile sample;
	PDB_HANDLE h;
	if(check_args_num(argc,2)){
		printf("Usage: n name\nnamet\tname of user/group\n");
		return;
	}
	PDB_lookupByName(argv[1], &id);
	h = PDB_db_open(O_RDONLY);
	PDB_readProfile(h, id, &sample);
	PDB_printProfile(stdout, &sample);
	PDB_freeProfile(&sample);
	PDB_db_close(h);
}


/* LIST EVERTHING */
void tool_list(int argc,char *argv[]){
	int32_t id;
	PDB_profile sample;
	PDB_HANDLE h;
	int rc;

	if(check_args_num(argc,1)){
		printf("Usage: list\n");
		return;
	}

	h = PDB_db_open(O_RDONLY);
	while ( (rc = PDB_db_nextkey(h, &id))) {
		if ( rc == -1 )
			continue; 
		PDB_readProfile(h, id, &sample);
		PDB_printProfile(stdout, &sample);
		PDB_freeProfile(&sample);
	}
	PDB_db_close(h);
}


/* CREATE NEW USER */
void tool_newUser(int argc,char *argv[]){
	int32_t id;
	if(check_args_num(argc,2)){
		printf("Usage: nu name\nname\t\tname of new user\n");
		return;
	}
	PDB_createUser(argv[1], &id);
}


/* CREATE NEW USER WITH ID*/
void tool_newUser_Id(int argc,char *argv[]){
	char *s;
	int32_t id;
	long arg2;
	if(check_args_num(argc,3)){
		printf("Usage: nui name id\nname\t\t"
		       "name of new user\nid\t\tid of new user\n");
		return;
	}
	arg2 = atol(argv[2]);
	if(arg2 == 0){
		printf("Give numerical value.\n");
		return;
	}
	PDB_lookupById((int32_t) arg2, &s);
	if(s != NULL){
		printf("ID alread used by \"%s\".\n",s);
		free(s);
		return;
	}
	free(s);
	PDB_createUser(argv[1], &id);
	if (id == 0){
		printf("Failed to creat user.\n");
		return;
	}
	if (id != arg2)
		PDB_changeId(id,arg2);
}


/* CREATE NEW USER */
void tool_changeName(int argc,char *argv[]){
	long arg1;
	if(check_args_num(argc,3)){
		printf("Usage: cn id name\nid\t\t"
		       "id number of user\n\nname\t\tnew name of user\n");
		return;
	}
	arg1 = atol(argv[1]);
	if(arg1 == 0){
		printf("Give numerical value.\n");
		return;
	}
	PDB_changeName(arg1, argv[2]);
}


/* CREATE NEW GROUP */
void tool_newGroup(int argc,char *argv[]){
	char *s;
	int32_t id;
	long arg2;
	if(check_args_num(argc,3)){
		printf("Usage: ng name owner\nname\t\t"
		       "name of new group\nowner\t\t"
		       "id number of group owner\n");
		return;
	}
	arg2 = atol(argv[2]);
	if(arg2 == 0){
		printf("Give numerical value.\n");
		return;
	}
	if(!PDB_ISUSER(arg2)){
		printf("Owner must be a user!\n");
		return;
	}
	PDB_lookupById((int32_t) arg2, &s);
	if(s == NULL){
		printf("No user %ld!\n",arg2);
		return;
	}
	free(s);
	PDB_createGroup(argv[1], arg2, &id);
}


void tool_newDefGroup(int argc, char **argv)
{
	int32_t ownerid, id;
	char *colon = NULL;
	if(check_args_num(argc,2)){
		printf("Usage: ng name\nname\t\t"
		       "owner:name - name of new group\n");
		return;
	}
	
	colon = strchr(argv[1], ':');
	if ( !colon ) {
		printf("Name must be of the form: owner:group\n");
		return;
	}

	*colon = '\0';
	PDB_lookupByName(argv[1], &ownerid);
	*colon = ':';

	if ( !ownerid ) {
		printf("Owner must exist!\n");
		return;
	}
		
	PDB_createGroup(argv[1], ownerid, &id);
	printf("Created %s with id %d\n", argv[1], id);

}




/* LOOK UP A USER OR GROUP NAME */
void tool_lookup(int argc,char *argv[]){
	int32_t id;
	if(check_args_num(argc,2)){
		printf("Usage: l id\nid\t\tid number of user/group\n");
		return;
	}
	PDB_lookupByName(argv[1], &id);
	printf("%s's id is %d\n",argv[1],id);
}

/* CLONE A USER */
void tool_clone(int argc,char *argv[]){
	int32_t id;
	long arg2;
	if(check_args_num(argc,3)){
		printf("Usage: cu name id\nname\t\t"
		       "name of new user\nid\t\tid number of user to clone\n");
		return;
	}
	arg2 = atol(argv[2]);
	if(arg2 == 0){
		printf("Give numerical value.\n");
		return;
	}
	PDB_cloneUser(argv[1], arg2, &id);
}

/* ADD SOMEONE (USER OR GROUP) TO A GROUP */
void tool_addtoGroup(int argc,char *argv[]){
	char *s;
	long arg1, arg2;
	if(check_args_num(argc,3)){
		printf("Usage: ag group user\ngroup\t\t"
		       "id of group to add to\nuser\t\t"
		       "id number of user to add\n");
		return;
	}
	arg1 = atol(argv[1]);
	if(arg1 == 0){
		printf("Give numerical value.\n");
		return;
	}
	PDB_lookupById((int32_t) arg1, &s);
	if(s == NULL){
		printf("No group %ld!\n",arg1);
		return;
	}
	free(s);
	arg2 = atol(argv[2]);
	if(arg2 == 0){
		printf("Give numerical value.\n");
		return;
	}
	PDB_lookupById((int32_t) arg2, &s);
	if(s == NULL){
		printf("No user %ld!\n",arg2);
		return;
	}
	free(s);
	PDB_addToGroup(arg2,arg1);
}

/* REMOVE SOMEONE (USER OR GROUP) TO A GROUP */
void tool_removefromGroup(int argc,char *argv[]){
	long arg1, arg2;
	if(check_args_num(argc,3)){
		printf("Usage: rg group user\ngroup\t\t"
		       "id of group to remove from\nuser\t\t"
		       "id number of user to remove\n");
		return;
	}
	arg1 = atol(argv[1]);
	if(arg1 == 0){
		printf("Give numerical value.\n");
		return;
	}
	arg2 = atol(argv[2]);
	if(arg2 == 0){
		printf("Give numerical value.\n");
		return;
	}
	PDB_removeFromGroup(arg2, arg1);
}

/* DELETE USER OR GROUP */
void tool_delete(int argc,char *argv[]){
	char *s;
	long arg1;
	if(check_args_num(argc,2)){
		printf("Usage: d id\nid\t\tid number of user/group\n");
		return;
	}
	arg1 = atol(argv[1]);
	if(arg1 == 0){
		printf("Give numerical value.\n");
		return;
	}
	PDB_lookupById((int32_t) arg1, &s);
	if(s == NULL){
		printf("No user %ld!\n",arg1);
		return;
	}
	free(s);
	if(PDB_ISGROUP(arg1))
		PDB_deleteGroup(arg1);
	else
		PDB_deleteUser(arg1);
}


/* UPDATE USER OR GROUP */
void tool_update(int argc,char *argv[]){
	long arg1;
	PDB_HANDLE h;
	PDB_profile p;
	if(check_args_num(argc,2)){
		printf("Usage: u id\nid\t\tid number of user/group\n");
		return;
	}
	h = PDB_db_open(O_RDWR);
	arg1 = atol(argv[1]);
	if(arg1 == 0){
		printf("Give numerical value.\n");
		return;
	}
	PDB_readProfile(h, arg1, &p);
	if(p.id != 0)
		PDB_writeProfile(h, &p);
	PDB_db_close(h);
}

/* COMPACT DATABASES */
void tool_compact(int argc,char *argv[]){
	PDB_HANDLE h;
	if(check_args_num(argc,1)){
		printf("Usage: cm\n");
		return;
	}
	h = PDB_db_open(O_RDWR);
	PDB_db_compact(h);
	PDB_db_close(h);
}

/* SHOW MAXIDS */
void tool_get_maxids(int argc,char *argv[]){
      PDB_HANDLE h;
      int maxuid, maxgid;
      if(check_args_num(argc,1)){
              printf("Usage: get_maxids\n");
      }
      h = PDB_db_open(O_RDWR);
      PDB_db_maxids(h, &maxuid, &maxgid);
      PDB_db_close(h);
      printf("maxuid %d maxgid %d\n", maxuid, maxgid);
}

/* SET MAXIDS */
void tool_maxids(int argc,char *argv[]){
	PDB_HANDLE h;
	long arg1, arg2;
	if(check_args_num(argc,3)){
		printf("Usage: maxids usermax groupmin\n");
		return;
	}
	arg1 = atol(argv[1]);
	if(arg1 == 0){
		printf("Give numerical value.\n");
		return;
	}
	arg2 = atol(argv[2]);
	if(arg2 == 0){
		printf("Give numerical value.\n");
		return;
	}
	h = PDB_db_open(O_RDWR);
	PDB_db_update_maxids(h, arg1, arg2, PDB_MAXID_FORCE);
	PDB_db_close(h);
}

/* CHANGE ID */
void tool_changeId(int argc,char *argv[])
{
	int32_t id;
	long arg2;
	if(check_args_num(argc,3)) {
		printf("Usage %s username newid\n", argv[0]);
		return;
	}
	arg2 = atol(argv[2]);
	if(arg2 == 0){
		printf("Give numerical value.\n");
		return;
	}
	PDB_lookupByName(argv[1], &id);
	if (id == 0){
		printf("Invalid user.\n");
		return;
	}
	PDB_changeId(id,arg2);
}

/* dump/restore database contents */
void tool_export(int argc, char *argv[])
{
    int32_t id, i;
    PDB_profile rec;
    PDB_HANDLE h;
    FILE *userfile, *groupfile;
    char *s;
    int rc;

    if (check_args_num(argc, 3)) {
	printf("Usage: export <userfile> <groupfile>\n");
	return;
    }

    userfile  = fopen(argv[1], "w");
    groupfile = fopen(argv[2], "w");

    h = PDB_db_open(O_RDONLY);
    while ((rc = PDB_db_nextkey(h, &id))) {
	if (rc == -1) continue;

	PDB_readProfile(h, id, &rec);
	{
	    if (PDB_ISUSER(rec.id)) {
		/* users are dumped in an /etc/passwd like format
		 * "<username>:x:<userid>:500::/:" */
		fprintf(userfile, "%s:*:%d:500::/:\n", rec.name, rec.id);
	    } else {
		/* groups and group members are dumped in an /etc/group like
		 * format "<groupname>:x:<groupid>:<owner>[,<members>]*" */

		/* escape the :'s in the group names */
		s = rec.name; while ((s = strchr(s, ':')) != NULL) *s = '%';

		fprintf(groupfile, "%s:*:%d:%s", rec.name, rec.id,
						 rec.owner_name);
		for (i = 0; i < rec.groups_or_members.size; i++) {
		    if (rec.groups_or_members.data[i] == rec.owner_id)
			continue;

		    PDB_lookupById(rec.groups_or_members.data[i], &s);
		    if (s == NULL) continue;

		    fprintf(groupfile, ",%s", s);
		    free(s);
		}
		fprintf(groupfile, "\n");
	    }
	}
	PDB_freeProfile(&rec);
    }
    PDB_db_close(h);

    fclose(userfile);
    fclose(groupfile);
}

void tool_import(int argc, char *argv[])
{
    FILE *userfile, *groupfile;
    char user[64], group[64], owner_and_members[1024], *owner, *member, *s;
    int32_t user_id, group_id, owner_id, member_id, create_id;
    int rc;

    if (check_args_num(argc, 3)) {
	printf("Usage: import <userfile> <groupfile>\n");
	return;
    }

    /* recreate all users */
    userfile = fopen(argv[1], "r");
    while(1) {
	rc = fscanf(userfile, "%[^:]:%*[^:]:%d:%*s\n", user, &user_id);
	if (rc < 0) break;

	/* create user */
	PDB_lookupById(user_id, &s);
	if (s) {
	    printf("Duplicate user for id %d, found both %s and %s\n",
		   user_id, s, user);
	    free(s);
	    continue;
	}

	PDB_createUser(user, &create_id);
	PDB_changeId(create_id, user_id);
	printf("Created user %s, id %d\n", user, user_id);
    }
    fclose(userfile);
    
    /* recreate groups */
    groupfile = fopen(argv[2], "r");
    while (1) {
	rc = fscanf(groupfile, "%[^:]:%*[^:]:%d:%s\n",
		    group, &group_id, owner_and_members);
	if (rc < 0) break;

	/* restore the :'s in the group name */
	s = group; while ((s = strchr(s, '%')) != NULL) *s = ':';

	owner = strtok(owner_and_members, ",");

	/* create group */
	PDB_lookupByName(owner, &owner_id);
	if (owner_id == 0) {
	    printf("Group %s's owner %s cannot be found\n", group, owner);
	    continue;
	}
	if (!PDB_ISUSER(owner_id)) {
	    printf("Group %s's owner %s is a group but should be a user\n",
		   group, owner);
	    continue;
	}
	PDB_createGroup(group, owner_id, &create_id);
	PDB_changeId(create_id, group_id);
	printf("Created group %s, id %d, owner %s\n", group, group_id, owner);
    }   

    /* add group members*/
    rewind(groupfile);
    while (1) {
	rc = fscanf(groupfile, "%[^:]:%*[^:]:%d:%s\n",
		    group, &group_id, owner_and_members);
	if (rc < 0) break;

	/* restore the :'s in the group name */
	s = group; while ((s = strchr(s, '%')) != NULL) *s = ':';

	/* skip the owner */
	(void)strtok(owner_and_members, ",");

	/* add group members */
	printf("Adding members to %s\n\t", group);
	while ((member = strtok(NULL, ",")) != NULL) {
	    /* restore the :'s in the name */
	    s = member; while ((s = strchr(s, '%')) != NULL) *s = ':';

	    PDB_lookupByName(member, &member_id);
	    if (member_id == 0) {
		printf("\nGroup %s's member %s cannot be found\n\t",
		       group, member);
		continue;
	    }
	    PDB_addToGroup(member_id, group_id);
	    printf(" %s", member);
	}
	printf("\n");
    }   
    fclose(groupfile);
}

void tool_source(int argc, char *argv[])
{
	char line[1024];
	char *nl;

	FILE *file = fopen(argv[1], "r");
	if ( !file ) {
	    perror("");
	    return;
	}
	while ( fgets(line, 1024, file) ) {
	    if ( (nl = strchr(line, '\n')) )
		*nl = '\0';
	    execute_line(line);
	}
}


/* HELP */
void tool_help(int argc, char *argv[])
{
	if (argc > 1) {
	    Parser_help(argc, argv);
	    return;
	}

        printf("i <id>\t\t\t\tread database by ID\n");
	printf("n <name>\t\t\tread database by name\n");
	printf("nu <username>\t\t\tcreate a new user\n");
	printf("nui <username> <userid>\t\tcreate a new user with id\n");
	printf("ng <groupname> <ownerid>\tcreate a new group\n");
	printf("l <name>\t\t\tlook up an ID by name\n");
	printf("list\t\t\t\tlist all entries\n");
	printf("cu <newusername> <userid>\tclone a user\n");
	printf("ag <groupid> <id>\t\tadd a group or user to a group\n");
	printf("rg <groupid> <id>\t\tremove a group or user from a group\n");
	printf("d <id>\t\t\t\tdelete a user or a group\n");
	printf("cm\t\t\t\tcompact the database (RARE)\n");
	printf("ci <name> <newid>\t\tchange the Id of a user or group\n");
	printf("cn <id> <newname>\t\tchange the Name of a user or group\n");
	printf("u <id>\t\t\t\tupdate an id\n");
	printf("ids\t\t\t\tget the database maxids\n");
	printf("maxids <userid> <groupid>\tset the database maxids\n");
	printf("export <userfile> <groupfile>\tdump the contents of the pdb database\n");
	printf("import <userfile> <groupfile>\tread a dumped pdb database\n");
	printf("source <file>\t\t\tread commands from file\n");
}

command_t pdbcmds[] =
{
        {"i", tool_byId, 0, "read database by user ID"},
	{"n", tool_byName, 0, "read database by user name"},
	{"nu", tool_newUser, 0, "create a new user"},
	{"nui", tool_newUser_Id, 0, "create a new user with id"},
	{"ng", tool_newGroup, 0, "create a new group"},
	{"newgroup", tool_newDefGroup, 0, "create a new group"},
	{"l", tool_lookup, 0, "look up an ID by name"},
	{"list", tool_list, 0, "list all entries"},
	{"cu", tool_clone, 0, "clone a user"},
	{"ag", tool_addtoGroup, 0, "add a group or user to a group"},
	{"rg", tool_removefromGroup, 0, "remove a group or user from a group"},
	{"d", tool_delete, 0, "delete a user or a group"},
	{"cm", tool_compact, 0, "compact the database (RARE)"},
	{"ci", tool_changeId, 0, "change the Id of a user or group"},
	{"cn", tool_changeName, 0, "change the Name of a user"},
	{"u", tool_update, 0, "update an id"},
	{"ids", tool_get_maxids, 0, "get the database maxids"},
	{"maxids", tool_maxids, 0, "set the database maxids"},
	{"export", tool_export, 0, "dump the contents of the database"},
	{"import", tool_import, 0, "load the contents of the database"},
	{"source", tool_source, 0, "read commands from file"},
	{"help", tool_help, 0, "print help on commands"},
	{"quit", Parser_exit, 0, "get me out of here"},
	{"exit", Parser_exit, 0, "get me out of here"},
	{ 0, 0, 0, NULL }
};


int main(int argc, char **argv)
{
	int i;
	coda_assert_action = CODA_ASSERT_EXIT;

	PDB_setupdb();
	Parser_init("pdbtool> ", pdbcmds);
	if ( argc == 1 )
		Parser_commands();
	else {
		char line[1024];
		strcpy(line, argv[1]);
		for (i = 2; i < argc; i++) {
		    strcat(line, " ");
		    strcat(line, argv[i]);
		}
		execute_line(line);
	}
	return 0;
}
