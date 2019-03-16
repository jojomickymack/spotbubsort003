package org.central;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class SpotifyGetTrack implements Runnable {
    private String response = "no response received";
    private String tracksEndpoint = "https://api.spotify.com/v1/tracks/";
    private String trackId = "";
    private String token = "";

    public SpotifyGetTrack (String token) {
        this.token = token;
    }

    public void setTrackId(String trackId) {
        this.trackId = trackId;
    }

    @Override
    public void run() {
        if (token == "") {
            this.response = "no token";
        } else {
            try {
                URL url = new URL(tracksEndpoint + trackId);
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

