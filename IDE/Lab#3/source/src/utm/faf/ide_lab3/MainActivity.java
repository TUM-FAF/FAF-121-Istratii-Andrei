package utm.faf.ide_lab3;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.UUID;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.support.v7.app.ActionBarActivity;
import android.support.v7.app.ActionBar;
import android.support.v4.app.Fragment;
import android.text.format.DateFormat;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.os.Build;

public class MainActivity extends ActionBarActivity {
	
	EditText etResponse;
    TextView tvIsConnected;
    ImageView homeLogo;
    ImageView awayLogo;
    
    TextView homeTextView;
    TextView awayTextView;
    
    TextView homeScore;
    TextView awayScore;
    
    TextView timeText;
    
    LinearLayout container;
    
    ArrayList<Match> matches;
    ArrayList<Team> teams;
    
    int currentMatch;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        matches = new ArrayList<Match>();
        teams = new ArrayList<Team>();
        
     // get reference to the views
        tvIsConnected = (TextView) findViewById(R.id.tvIsConnected);
        homeLogo = (ImageView) findViewById(R.id.homeLogo);
        awayLogo = (ImageView) findViewById(R.id.awayLogo);
        
        homeTextView = (TextView) findViewById(R.id.homeTextView);
        awayTextView = (TextView) findViewById(R.id.awayTextView);
        
        homeScore = (TextView) findViewById(R.id.homeScore);
        awayScore = (TextView) findViewById(R.id.awayScore);
        
        timeText = (TextView) findViewById(R.id.timeText);
        
        container = (LinearLayout) findViewById(R.id.uiContainer);
        container.setVisibility(View.INVISIBLE);
 
        // check if you are connected or not
        if(isConnected()) {
            tvIsConnected.setBackgroundColor(0xFF00CC00);
            tvIsConnected.setText("You are connected");
        } else {
            tvIsConnected.setText("You are NOT connected");
        }
               
        
		
        String url = "http://worldcup.kimonolabs.com/api/matches";
        String apikey = "?apikey=e9bda7a4d1a7609a33867deadc915309";
        Log.d("URL", url+apikey);
        new FetchWorldCupData().execute(); 
    }
    
    public void prevMatch(View v) {
    	currentMatch--;
    	if (currentMatch < 0) {
    		currentMatch = 0;
    	}
    	updateMatch();
    }
    
    public void nextMatch(View v) {
    	currentMatch++;
    	if (currentMatch >= matches.size()) {
    		currentMatch = matches.size()-1;
    	}
    	updateMatch();
    }
    
    public void updateMatch() {
    	Match m = matches.get(currentMatch);
    	SimpleDateFormat df = new SimpleDateFormat("dd-MM-yyyy\nHH:mm:ss");
    	timeText.setText(df.format(m.startTime));
    	for (Team t : teams) {
    		if (t.id.equals(m.homeTeamId)) {
    			homeTextView.setText(t.name);
    			homeLogo.setImageBitmap(t.getLogoBitmap());
    			homeScore.setText(Integer.toString(m.homeScore));
    		}
    		
    		if (t.id.equals(m.awayTeamId)) {
    			awayTextView.setText(t.name);
    			awayLogo.setImageBitmap(t.getLogoBitmap());
    			awayScore.setText(Integer.toString(m.awayScore));
    		}
    	}
    }
    
    public void findLastMatch() {
    	Calendar calendar = Calendar.getInstance();
    	Date now = calendar.getTime();
    	currentMatch = 0;
    	for (int i=0; i<matches.size(); i++) {
    		currentMatch++;
    		if (matches.get(i).startTime.compareTo(now) > 0) {
    			break;
    		}
    	}
    	
    }
    
    public static String GET(String url) {
        InputStream inputStream = null;
        String result = "";
        try {
 
            // create HttpClient
            HttpClient httpclient = new DefaultHttpClient();
 
            // make GET request to the given URL
            HttpResponse httpResponse = httpclient.execute(new HttpGet(url));
 
            // receive response as inputStream
            inputStream = httpResponse.getEntity().getContent();
 
            // convert inputstream to string
            if(inputStream != null)
                result = convertInputStreamToString(inputStream);
            else
                result = "Did not work!";
 
        } catch (Exception e) {
            Log.d("InputStream", e.getLocalizedMessage());
        }
 
        return result;
    }
 
    private static String convertInputStreamToString(InputStream inputStream) throws IOException{
        BufferedReader bufferedReader = new BufferedReader( new InputStreamReader(inputStream));
        String line = "";
        String result = "";
        while((line = bufferedReader.readLine()) != null)
            result += line;
 
        inputStream.close();
        return result;
 
    }
    
 
    public boolean isConnected(){
        ConnectivityManager connMgr = (ConnectivityManager) getSystemService(this.CONNECTIVITY_SERVICE);
            NetworkInfo networkInfo = connMgr.getActiveNetworkInfo();
            if (networkInfo != null && networkInfo.isConnected()) 
                return true;
            else
                return false;   
    }
    
    private class FetchWorldCupData extends AsyncTask<Void, Void, String[]> {
        @Override
        protected String[] doInBackground(Void... v) {
        	String[] result = new String[2];
        	String baseUrl = "http://worldcup.kimonolabs.com/api/";
        	String apikey = "&apikey=e9bda7a4d1a7609a33867deadc915309";
        	result[0] = GET(baseUrl + "teams?fields=name,logo,id" + apikey);
        	result[1] = GET(baseUrl + "matches?fields=homeScore,awayScore,startTime,awayTeamId,homeTeamId&sort=startTime" + apikey);
            return result;
        }
        // onPostExecute displays the results of the AsyncTask.
        @Override
        protected void onPostExecute(String[] returnedData) {
        	Log.d("Teams", returnedData[0]);
        	Log.d("Matches", returnedData[1]);
        	
        	try {
				JSONArray teamsJson = new JSONArray(returnedData[0]);
				JSONArray matchesJson = new JSONArray(returnedData[1]);
				
				for (int i=0; i<teamsJson.length(); i++) {
					teams.add(new Team(teamsJson.getJSONObject(i)));
					
				}
				
				for (int i=0; i<matchesJson.length(); i++) {
					matches.add(new Match(matchesJson.getJSONObject(i)));
				}
				new FetchBitmapFromWeb().execute();
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
        	
        }
    }
    
    
    private class FetchBitmapFromWeb extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... v) {
        	try {
        		for(Team t : teams) {
        			Bitmap bmp = BitmapFactory.decodeStream((InputStream)new URL(t.getLogoUrl()).getContent());
        			t.attachLogoBitmap(bmp);
        		}        		
    		} catch (MalformedURLException e) {
    			e.printStackTrace();
    		} catch (IOException e) {
    			e.printStackTrace();
    		}
			return null;
        }
        
     // onPostExecute displays the results of the AsyncTask.
        @Override
        protected void onPostExecute(Void v) {
        	findLastMatch();
        	updateMatch();
        	container.setVisibility(View.VISIBLE);
        }

    }

}
