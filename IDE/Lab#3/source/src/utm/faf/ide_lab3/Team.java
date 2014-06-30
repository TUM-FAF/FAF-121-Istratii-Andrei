package utm.faf.ide_lab3;

import java.util.UUID;

import org.json.JSONException;
import org.json.JSONObject;

import android.graphics.Bitmap;

public class Team {
	
	public UUID id;
	public String name;
	private Bitmap logo;
	private String logoUrl;
	
	
	public Team(JSONObject teamInfo) {
		try {
			id = UUID.fromString(teamInfo.getString("id"));
			name = teamInfo.getString("name");
			logoUrl = teamInfo.getString("logo");
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	public String getLogoUrl() {
		return logoUrl;
	}
	
	public Bitmap getLogoBitmap() {
		return logo;
	}
	
	public void attachLogoBitmap(Bitmap bmp) {
		logo = bmp;
	}
	
}
