char* BuildPath(char* str)
{
	char* ret_str = str;

	if ((strlen(ret_str) == 1) && (strcmp(ret_str, "/") == 0))
		return ret_str;

	// check for ~
	if (ret_str[0] == '~')
	{
		ret_str = ReplaceSubStr(ret_str, 0, 0, getenv("HOME"));
	}

	// check for .
	if (ret_str[0] == '.' && ret_str[1] != '.')
	{
		// if in root, need to delete both . and /
		if (strcmp(getenv("PWD"), "/") == 0)
			ret_str = ReplaceSubStr(ret_str, 0, 1, getenv("PWD"));
		// else, only need to delete .
		else
			ret_str = ReplaceSubStr(ret_str, 0, 0, getenv("PWD"));
	}

	// check for ..
	if (ret_str[0] == '.' && ret_str[1] == '.')
	{
		char* pwd = getenv("PWD");
		char* dyn_pwd = (char*)calloc(strlen(pwd)+1,sizeof(char));
		strcpy(dyn_pwd, pwd);
		dyn_pwd = RemoveLastDir(dyn_pwd);
		// attempt to hardcode fix changing to / from /home
		if (strcmp(dyn_pwd,"")==0)
		{
			ret_str = ReplaceSubStr(ret_str, 0, strlen(ret_str)-1, "/");
			free(dyn_pwd);
			return ret_str;
		}
		ret_str = ReplaceSubStr(ret_str, 0, 1, dyn_pwd);
		free(dyn_pwd);
	}

	// check for . and .. anywhere in string
	size_t it = 1;
	size_t last_slash = 0;
	size_t last_last_slash = 0;
	while (ret_str[it] != '\0')
	{
		if (ret_str[it] == '.' && ret_str[it-1] == '.')
		{
			// check for root
			if (it > 2)
			{
				ret_str = DeleteCharacter(ret_str, last_last_slash, it);
				// must start back at 0 to capture '/' locations
				it = 0;
				last_slash = 0;
				last_last_slash = 0;
				// empty string, down to root
				if (strcmp(ret_str,"") == 0)
				{
					ret_str = DynStrPushBack(ret_str, '/');
					return ret_str;
				}
			}
			else if (strlen(ret_str) > 3)
			{
				ret_str = DeleteCharacter(ret_str, it-2, it);
				it = it - 3;	// will be -1, ++it at end of loop will make 0
			}
			else
			{
				ret_str = DeleteCharacter(ret_str, it-1, it);
				it = it - 2;
			}
		}
		// check strlen to make sure no invalid memory read
		else if ((strlen(ret_str) >= 2) && ret_str[it-1] == '.' && ret_str[it] == '/')
		{
			ret_str = DeleteCharacter(ret_str, it-1, it);
			it = it - 2;
		}
		else if (ret_str[it] == '/')
		{
			last_last_slash = last_slash;
			last_slash = it;
		}
		++it;
	}

	return ret_str;
}
char* ReplaceSubStr(char* dest, size_t start, size_t end, const char* source)
{
	size_t deletion_len = end - start + 1;
	size_t dest_len = strlen(dest);
	size_t source_len = strlen(source);
	size_t new_len = dest_len - deletion_len + source_len;

	char* ret_str = (char*)calloc(new_len + 1, sizeof(char));
	ret_str[new_len - 1] = '\0';

	int ret_str_it = 0;
	int dest_it = 0;
	int source_it = 0;
	int final_it = 0;

	if (start == 0)
		final_it = end + 1;
	else
		final_it = end + 1;

	for (ret_str_it = 0; ret_str_it < new_len; ret_str_it++)
	{
		if (ret_str_it < start)
			ret_str[ret_str_it] = dest[dest_it++];
		else if ((ret_str_it >= start) && (ret_str_it <= start+source_len-1))
			ret_str[ret_str_it] = source[source_it++];
		else
			ret_str[ret_str_it] = dest[final_it++];
	}
	free(dest);
	return ret_str;
}

int ContainsChar(const char* str, char c)
{
	size_t it = 0;
	while (str[it] != '\0')
	{
		if (str[it] == c)
		{
			return 1;
		}
		++it;
	}
	return 0;
}

int VecContainsStr(char** argv, const char* str)
{
	int it = 0;
	while (argv[it] != NULL)
	{
		if (strcmp(argv[it], str) == 0)
		{
			return it;
		}
		++it;
	}
	return -1;
}

int ArraySize(char** argv)
{
	if (argv == NULL)
		return 0;
	int it = 0;
	while(argv[it] != NULL)
	{
		++it;
	}
	return it;
}

char** ArrayPushBack(char** argv, const char* str)
{
	char** new_argv = (char**)calloc(ArraySize(argv)+2,sizeof(char*));
	int it = 0;
	while (argv[it] != NULL)
	{
		new_argv[it] = (char*)calloc(strlen(argv[it])+1, sizeof(char));
		strcpy(new_argv[it], argv[it]);
		++it;
	}
	new_argv[it] = (char*)calloc(strlen(str)+1, sizeof(char));
	strcpy(new_argv[it], str);
	new_argv[it+1] = NULL;
	BigFree(argv);
	return new_argv;
}

char** ArrayRemoveElement(char** argv, int index)
{
	if (index > ArraySize(argv))
	{
		return argv;
	}
	int it1 = 0;
	int it2 = 0;

	char** new_argv = (char**)calloc(ArraySize(argv),sizeof(char*));
	while (argv[it1] != NULL)
	{
		if (it1 != index)
		{
			new_argv[it2] = (char*)calloc(strlen(argv[it1])+1, sizeof(char));
			strcpy(new_argv[it2], argv[it1]);
			++it2;
		}
		++it1;
	}
	new_argv[it2] = NULL;
	BigFree(argv);
	return new_argv;
}
