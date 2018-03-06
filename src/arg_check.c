int arg_check_detail(char *str, int num[]){
  
  return 0;
}

int arg_check(int ac, char **av, int num[]){
  int i;

  if (ac == 1) return 1;

  for (i=0 ; i<ac; i++){
    arg_check_detail(av[i], num);
  }

  return 0;
}
