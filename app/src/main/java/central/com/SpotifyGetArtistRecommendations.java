package central.com;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class SpotifyGetArtistRecommendations implements Runnable {
    private String response = "no response received";
    private String recommendationsEndpoint = "https://api.spotify.com/v1/recommendations";
    private String artistId = "";
    private String token = "";

    public SpotifyGetArtistRecommendations (String token) {
        this.token = token;
    }

    public void setArtistId(String artistId) {
        this.artistId = artistId;
    }

    @Override
    public void run() {
        if (token == "") {
            this.response = "no token";
        } else {
            try {
                URL url = new URL(recommendationsEndpoint + "?seed_artists=" + artistId);
                HttpURLConnection conn = (HttpURLConnection) url.openConnection();
                conn.setRequestProperty("Authorization", "Bearer " + this.token);
                try {
                    InputStream is = new BufferedInputStream(conn.getInputStream());
                    int ch;
                    StringBuilder sb = new StringBuilder();
                    while ((ch = is.read()) != -1) {
                        sb.append((char) ch);
                    }
                    this.response = sb.toString();
                } finally {
                    conn.disconnect();
                }
            } catch (IOException e) {
                System.out.println("there was an error " + e);
            }
        }
    }

    public String getResponse() {
        return this.response;
    }
}
