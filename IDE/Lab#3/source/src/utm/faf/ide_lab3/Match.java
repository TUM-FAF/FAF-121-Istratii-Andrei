package utm.faf.ide_lab3;

import java.text.ParseException;
import java.util.Date;
import java.util.UUID;

import org.json.JSONException;
import org.json.JSONObject;


public class Match {
	
	public UUID homeTeamId;
	public UUID awayTeamId;
	
	public int homeScore;
	public int awayScore;
	
	Date startTime;
	
	public Match(JSONObject matchInfo) {
		try {
			homeTeamId = UUID.fromString(matchInfo.getString("homeTeamId"));
			awayTeamId = UUID.fromString(matchInfo.getString("awayTeamId"));
			
			homeScore = matchInfo.getInt("homeScore");
			awayScore = matchInfo.getInt("awayScore");
			
			String timeStr = matchInfo.getString("startTime");
			startTime = ISO8601DateParser.parse(timeStr);
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (ParseException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
