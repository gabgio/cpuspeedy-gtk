/*
 * gtk-cpuspeedy
 * http://cpuspeedy.sourceforge.net
 *
 * Copyright (C) Gabriele Giorgetti <gabriele_giorgetti@tin.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <gtk/gtk.h>

#include "interface.h"
#include "support.h"
#include "callbacks.h"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define NO_PATH_CHECK_LONG_OPT  "--no-path-check"
#define NO_VERSION_CHECK_LONG_OPT  "--no-version-check"

#define CPUSPEEDY_PATH_SHORT_OPT "-p"
#define CPUSPEEDY_PATH_LONG_OPT  "--cpuspeedy-path="

#define HELP_SHORT_OPT    "-h"
#define HELP_LONG_OPT     "--help"

#define VERSION_SHORT_OPT "-v"
#define VERSION_LONG_OPT  "--version"

#define EMAIL "<gabriele_giorgetti@tin.it>"

/* 30 seconds */
#define UPDATE_STATUS_TIMEOUT 30000
/* cpuspeedy version 0.2 or later is required */
#define CPUSPEEDY_REQUIRED_VERSION "0.2.0"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void show_version ( void )
{
    fprintf (stdout, "%s %s\n", PACKAGE, VERSION);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void show_help ( void )
{
  fprintf (stdout, "%s, a gtk frontend for cpuspeedy. Version %s\n", PACKAGE, VERSION);

  fprintf (stdout, "\n");
  fprintf (stdout, "Usage: %s [OPTIONS]\n", PACKAGE);
  fprintf (stdout, "\n");
  fprintf (stdout, "Example: %s %s/usr/local/bin/cpuspeedy\n", PACKAGE, CPUSPEEDY_PATH_LONG_OPT);
  fprintf (stdout, "\n");
  fprintf (stdout, "Options:\n");
  fprintf (stdout, "      %s             disable cpuspeedy path checking\n", NO_PATH_CHECK_LONG_OPT);
  fprintf (stdout, "      %s          disable cpuspeedy version checking\n", NO_VERSION_CHECK_LONG_OPT);
  fprintf (stdout, "      %s,  %sPATH  path to cpuspeedy\n", CPUSPEEDY_PATH_SHORT_OPT, CPUSPEEDY_PATH_LONG_OPT);
  fprintf (stdout, "      %s,  %s                 display this help and exit\n", HELP_SHORT_OPT, HELP_LONG_OPT);
  fprintf (stdout, "      %s,  %s              output version information and exit\n", VERSION_SHORT_OPT, VERSION_LONG_OPT);
  fprintf (stdout, "\n");
  fprintf (stdout, "Report bugs to %s.\n", EMAIL );
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void parse_args ( int argc, char *argv[] )
{
  int i;
  char *tmpbuff;
  for (i = 1; i <= argc - 1; i++)
    {
      if ((!strcmp(argv[i], VERSION_SHORT_OPT)) || (!strcmp (argv[i], VERSION_LONG_OPT)))
        {
          show_version ();
          exit (EXIT_SUCCESS);
        }
      else if ((!strcmp (argv[i], HELP_SHORT_OPT)) || (!strcmp (argv[i], HELP_LONG_OPT)))
        {
          show_help ();
          exit (EXIT_SUCCESS);
        }
      else if ((!strcmp(argv[i], NO_PATH_CHECK_LONG_OPT)))
        {
          CPUSPEEDY_CHECK_PATH=FALSE;
        }
      else if ((!strcmp(argv[i], NO_VERSION_CHECK_LONG_OPT)))
        {
          CPUSPEEDY_CHECK_VERSION=FALSE;
        }
      else if ((strcmp(argv[i],CPUSPEEDY_PATH_SHORT_OPT)==0))
       {
           if (++i == argc)
            {
              fprintf (stderr, "%s: error, no path given\n", PACKAGE);
              fprintf (stderr, "Try `%s %s' for more information.\n", PACKAGE, HELP_LONG_OPT);
              exit (EXIT_FAILURE);
            }          
           strncpy (CPUSPEEDY_PATH, argv[i],sizeof(CPUSPEEDY_PATH));
           CPUSPEEDY_HAVE_PATH=TRUE;
       }       
      else if ((tmpbuff=strstr(argv[i],CPUSPEEDY_PATH_LONG_OPT))!=NULL)
        {
            char *tmpbuff;
            tmpbuff=strtok(argv[i], "=");
            if (tmpbuff!=NULL)
            {
                tmpbuff=strtok(NULL, "=");
                strncpy (CPUSPEEDY_PATH, tmpbuff,sizeof(CPUSPEEDY_PATH));
                CPUSPEEDY_HAVE_PATH=TRUE;
            }
        }           
      else if ((tmpbuff=strstr(argv[i],CPUSPEEDY_PATH_SHORT_OPT))!=NULL) 
        {
            char *tmpbuff;
            tmpbuff=strtok(argv[i],CPUSPEEDY_PATH_SHORT_OPT);
            if (tmpbuff!=NULL)
            {
                tmpbuff=strtok(NULL,CPUSPEEDY_PATH_SHORT_OPT);
                strncpy (CPUSPEEDY_PATH, tmpbuff,sizeof(CPUSPEEDY_PATH));
                CPUSPEEDY_HAVE_PATH=TRUE;
            }
        }
    }
}  

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
gint cpuspeedy_path_check (void)
{
    FILE *fp;
    if (CPUSPEEDY_HAVE_PATH)
    {
     if ((fp=fopen(CPUSPEEDY_PATH,"r")) != NULL)
      {
        fclose(fp);
        return TRUE;
      }
      else
      {
        /* given path is not a valid path */
        CPUSPEEDY_HAVE_PATH=FALSE;
      }
    }
    if ((fp=fopen("/usr/sbin/cpuspeedy","r")) != NULL)
    {
        fclose(fp);
        strncpy (CPUSPEEDY_PATH,"/usr/sbin/cpuspeedy",sizeof(CPUSPEEDY_PATH));
        CPUSPEEDY_HAVE_PATH=TRUE;
        return TRUE;
    }
    if ((fp=fopen("/usr/local/sbin/cpuspeedy","r")) != NULL)
    {
        fclose(fp);
        strncpy (CPUSPEEDY_PATH,"/usr/local/sbin/cpuspeedy",sizeof(CPUSPEEDY_PATH));
        CPUSPEEDY_HAVE_PATH=TRUE;
        return TRUE;
    }
    if (system ("type cpuspeedy 2> /dev/null > /dev/null") == 0)
        return TRUE;
    return FALSE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
gint cpuspeedy_version_check (void)
{
  FILE *pipe;
  char line[1024];
  char *package, *version;
    
  if (CPUSPEEDY_HAVE_PATH)
    {
        char execbuff[1024];
        strncpy(execbuff,CPUSPEEDY_PATH,sizeof(execbuff));
        strcat (execbuff, " --version");
        pipe=popen (execbuff, "r");
    }
  else
      pipe=popen ("cpuspeedy --version", "r");
  
  if (pipe == NULL)
    return FALSE;
  
  fgets (line, sizeof(line), pipe);
  pclose (pipe);
  
  package = strtok (line, " ");
  if (strcmp(package,"cpuspeedy")!=0)
      /* error not the expected --version output */
      return FALSE;
  
  version = strtok (NULL, " ");
  if (version == NULL)
       /* error not the expected --version output */
      return FALSE;

  /* this version is not supported */
  if (strcmp(version,"0.1")==0)
      return FALSE;
  
  /* this version is not supported */
  if (strcmp(version,"0.1.1")==0)
      return FALSE;
  
  return TRUE;
}

int main (int argc, char *argv[])
{
  GtkWidget *window_main, *dialog_error;
        
  CPUSPEEDY_HAVE_PATH=FALSE;
  CPUSPEEDY_CHECK_PATH=TRUE;  
  CPUSPEEDY_CHECK_VERSION=TRUE;
    
  parse_args(argc,argv);
    
  gtk_set_locale ();
  gtk_init (&argc, &argv);
    
  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");
  window_main = create_window_main ();
    
  if (getuid()!=0)
  {
     dialog_error = gtk_message_dialog_new (window_main,GTK_DIALOG_DESTROY_WITH_PARENT,
     GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE, "You must be the superuser (root) to use cpuspeedy.");
     gtk_dialog_run (GTK_DIALOG (dialog_error));
     gtk_widget_destroy (dialog_error);
     exit(EXIT_FAILURE);
     /*
     gtk_toggle_button_set_inconsistent (GTK_TOGGLE_BUTTON (lookup_widget(window_main,"radiobutton_min")),TRUE);
     gtk_toggle_button_set_inconsistent (GTK_TOGGLE_BUTTON (lookup_widget(window_main,"radiobutton_low")),TRUE);
     gtk_toggle_button_set_inconsistent (GTK_TOGGLE_BUTTON (lookup_widget(window_main,"radiobutton_med")),TRUE); 
     gtk_toggle_button_set_inconsistent (GTK_TOGGLE_BUTTON (lookup_widget(window_main,"radiobutton_high")),TRUE);
     gtk_toggle_button_set_inconsistent (GTK_TOGGLE_BUTTON (lookup_widget(window_main,"radiobutton_max")),TRUE);  
     */
  }  
  if ((CPUSPEEDY_CHECK_PATH) && (!cpuspeedy_path_check()))
  {
     dialog_error = gtk_message_dialog_new (window_main,GTK_DIALOG_DESTROY_WITH_PARENT,
     GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "cpuspeedy has not been found or is not in path.\nYou can download it from http://cpuspeedy.sourceforge.net\n\nType 'gtk-cpuspeedy --help' for more information.");
     gtk_dialog_run (GTK_DIALOG (dialog_error));
     gtk_widget_destroy (dialog_error);
     exit (EXIT_FAILURE);      
  } 
  if ((CPUSPEEDY_CHECK_VERSION) && (!cpuspeedy_version_check()))
  {
     dialog_error = gtk_message_dialog_new (window_main,GTK_DIALOG_DESTROY_WITH_PARENT,
     GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "A more recent version of cpuspeedy is required. You can download it from http://cpuspeedy.sourceforge.net\n\nType 'gtk-cpuspeedy --help' for more information.");
     gtk_dialog_run (GTK_DIALOG (dialog_error));
     gtk_widget_destroy (dialog_error);
     exit (EXIT_FAILURE);      
  } 
  /* get cpu status using cpuspeedy */
  cpuspeedy_status (window_main);
  /* display the main window */
  gtk_widget_show (window_main);
  /* get cpustatus every 30 seconds. */
  gtk_timeout_add(UPDATE_STATUS_TIMEOUT,cpuspeedy_status,window_main); 
  gtk_main ();
  return EXIT_SUCCESS;
}
