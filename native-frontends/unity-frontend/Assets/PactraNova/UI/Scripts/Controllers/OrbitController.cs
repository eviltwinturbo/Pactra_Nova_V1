using UnityEngine;
using System.Collections.Generic;

/// <summary>
/// OrbitController - Manages Orbit Messenger UI logic and state
/// Coordinates communication between UI panels and API client
/// Subscribes to WebSocket for realtime message updates
/// </summary>
public class OrbitController : MonoBehaviour
{
    [SerializeField] private GameObject SidebarPrefab;
    [SerializeField] private GameObject ConversationListPrefab;
    [SerializeField] private GameObject MessagePanePrefab;
    [SerializeField] private GameObject SettingsPanePrefab;
    [SerializeField] private GameObject UpgradeModalPrefab;
    [SerializeField] private GameObject TrialBannerPrefab;

    private GameObject sidebarInstance;
    private GameObject conversationListInstance;
    private GameObject messagePaneInstance;
    private GameObject settingsPaneInstance;
    private GameObject upgradeModalInstance;
    private GameObject trialBannerInstance;

    private ApiClient apiClient;
    private WebSocketClient webSocketClient;

    private void Start()
    {
        // Initialize API client
        apiClient = GetComponent<ApiClient>();
        if (apiClient == null)
        {
            apiClient = gameObject.AddComponent<ApiClient>();
        }

        // Get WebSocket client
        webSocketClient = GetComponent<WebSocketClient>();
        if (webSocketClient == null)
        {
            webSocketClient = gameObject.AddComponent<WebSocketClient>();
        }

        // Subscribe to WebSocket messages
        if (webSocketClient != null)
        {
            webSocketClient.OnMessageReceived += OnRealtimeMessageReceived;
            webSocketClient.OnConnected += OnWebSocketConnected;
            webSocketClient.OnDisconnected += OnWebSocketDisconnected;
            webSocketClient.OnError += OnWebSocketError;
        }

        // TODO: Initialize UI panels from prefabs
        // TODO: Load initial conversations
        // TODO: Set up event listeners
    }

    /// <summary>
    /// Send a message to the current conversation
    /// </summary>
    public new void SendMessage(string text)
    {
        if (apiClient != null)
        {
            // Will use WebSocket if connected, HTTP if not
            apiClient.SendMessage("default_conversation", text);
            
            // Append to local UI immediately (optimistic update)
            AppendMessageToUI(text, "You");
        }
    }

    /// <summary>
    /// Switch to a different conversation
    /// </summary>
    public void SelectConversation(string conversationId)
    {
        // TODO: Load messages for conversation
        // TODO: Initialize realtime connection for new conversation
        if (apiClient != null)
        {
            apiClient.InitializeRealtimeConnection(conversationId);
        }
    }

    /// <summary>
    /// Show upgrade modal if feature is tier-gated
    /// </summary>
    public void GateOrUpgrade(string featureId)
    {
        // TODO: Check tier against feature requirements
        // TODO: Show upgrade modal if necessary
    }

    /// <summary>
    /// Handle realtime message received from WebSocket
    /// </summary>
    private void OnRealtimeMessageReceived(string messagePayload)
    {
        // Parse JSON message payload
        // TODO: Extract author, text, timestamp from JSON
        
        Debug.Log($"Realtime message received: {messagePayload}");
        AppendMessageToUI(messagePayload, "Remote User");
    }

    /// <summary>
    /// Append message to UI (thread-safe, called on main thread)
    /// </summary>
    private void AppendMessageToUI(string messageText, string author)
    {
        string formattedMessage = $"[{author}]: {messageText}";
        Debug.Log($"Message appended to UI: {formattedMessage}");
        
        // TODO: Update message pane with new message
        // TODO: Ensure zero GC allocation by using object pool
    }

    private void OnWebSocketConnected()
    {
        Debug.Log("WebSocket connected - realtime messaging active");
    }

    private void OnWebSocketDisconnected()
    {
        Debug.LogWarning("WebSocket disconnected - falling back to polling");
    }

    private void OnWebSocketError(string error)
    {
        Debug.LogError($"WebSocket error: {error}");
    }

    private void OnDestroy()
    {
        // Unsubscribe from WebSocket events
        if (webSocketClient != null)
        {
            webSocketClient.OnMessageReceived -= OnRealtimeMessageReceived;
            webSocketClient.OnConnected -= OnWebSocketConnected;
            webSocketClient.OnDisconnected -= OnWebSocketDisconnected;
            webSocketClient.OnError -= OnWebSocketError;
        }

        // Shutdown realtime connection
        if (apiClient != null)
        {
            apiClient.ShutdownRealtimeConnection();
        }
    }
}
