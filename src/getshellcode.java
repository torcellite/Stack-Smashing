import java.io.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class getshellcode {
    
    private static List<String> primaryMatches;

    public static void main(String args[]) {
    
        String content = args[0];

        String entireLinePattern = "([a-f0-9]+\\:\\s*[a-f0-9\\s]+\\s*.+)";

        String hexCodePattern = "(?:[a-f0-9]+\\:\\s*)([a-f0-9\\s]+\\s{1})(?:\\s*)(.+)";

        Pattern r = Pattern.compile(entireLinePattern);
        Matcher m = r.matcher(content);
        primaryMatches = new ArrayList<String>();
        while(m.find()) {
            primaryMatches.add(m.group(0));
        }

        r = Pattern.compile(hexCodePattern);
        String hex[] = new String[primaryMatches.size()];
        String hexCode, actualCode;
        for(int i = 0; i < primaryMatches.size(); i++) {
            m = r.matcher(primaryMatches.get(i));
            if(m.find()) {
                hex[i] = "\"";
                hexCode = m.group(1);
                hexCode = hexCode.substring(hexCode.indexOf(':') + 1);
                hexCode = hexCode.trim();
                hexCode = hexCode.replace(" ", "\\x");
                hexCode = "\\x" + hexCode;
                hex[i] = hex[i] + hexCode + "\"";
                hex[i] = hex[i] + "\t// ";
                hex[i] = hex[i] + m.group(2);
            }
        }
        for(int i = 0; i < primaryMatches.size(); i++)
            System.out.println("\t"+hex[i]);
    }

}
