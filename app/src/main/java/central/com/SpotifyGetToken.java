package central.com;

import android.util.Log;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.util.*;

public class SpotifyGetToken implements Runnable {
    private String response = "no response received";
    private String clientId = "";
    private String clientSecret = "";
    private String tokenEndpoint = "https://accounts.spotify.com/api/token";

    public SpotifyGetToken(String clientId, String clientSecret) {
        this.clientId = clientId;
        this.clientSecret = clientSecret;
    }

    @Override
    public void run() {
        if (this.clientId == "" || this.clientSecret == "") {
            Log.d("SpotifyGetToken", "No App ClientId or ClientSecret Supplied!");
        } else {
            try {
                Map<String, Object> params = new LinkedHashMap<>();
                params.put("client_id", this.clientId);
                params.put("client_secret", this.clientSecret);
                params.put("grant_type", "client_credentials");

                StringBuilder postData = new StringBuilder();
                for (Map.Entry<String, Object> param : params.entrySet()) {
                    if (postData.length() != 0) postData.append('&');
                    postData.append(param.getKey());
                    postData.append('=');
                    postData.append(String.valueOf(param.getValue()));
                }

                byte[] postDataBytes = postData.toString().getBytes();

                URL url = new URL(tokenEndpoint);
                HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                conn.setRequestMethod("POST");
                conn.getOutputStream().write(postDataBytes);

                try {
                    InputStream is = new BufferedInputStream(conn.getInputStream());
                    int ch;
                    StringBuilder sb = new StringBuilder();
                    while ((ch = is.read()) != -1) {
                        sb.append((char) ch);
                    }
                    response = sb.toString();
                } finally {
                    conn.disconnect();
                }
            } catch(IOException e){
                System.out.println("there was an error " + e);
            }
        }
    }

    public String getResponse() {
        return response;
    }

}