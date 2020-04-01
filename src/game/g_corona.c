/*
===========================================================================
This file is part of shitposting, inappropriate entertainment and bad taste.
===========================================================================
*/

#include "g_local.h"

int reference = 1585720800; // April 1, 2020 8:00:00 (local time)

static const char *headlines[ ] =
{
  "Number of cases reaches %.0f in %s.",
  "Number of cases exceeds %.0f in %s.",
  "Number of infected people reaches %.0f in %s.",
  "BREAKING: %.0f confirmed cases in %s",
  "Coronavirus infects %.0f in %s"
};
int numHeadlines = sizeof( headlines ) / sizeof( const char* );

static const char *headlinesDeaths[ ] =
{
  "Number of deaths reaches %.0f in %s.",
  "Number of deaths exceeds %.0f in %s.",
  "Death toll climbs to %.0f in %s.",
  "BREAKING: %.0f now dead of coronavirus in %s",
  "Coronavirus kills %.0f in %s",
};
int numHeadlinesDeaths = sizeof( headlinesDeaths ) / sizeof( const char* );

static const char *countries[ ] =
{
  "Republic of Weimar",
  "the City State of Niveus",
  "People's Republic of Shit",
  "Paintfactoria",
  "Nintendo Land",
  "Spacetracks Conglomerate",
  "Nation of Shitplanet",
  "[REDACTED]",
};
int numCountries = sizeof( countries ) / sizeof( const char* );

static const char *people[ ] =
{
  "Prime Minister",
  "President",
  "Minister of Health",
  "Minister of Agriculture",
  "Vice-President",
  "Ambassador",
};
int numPeople = sizeof( people ) / sizeof( const char* );

static const char *quotes[] =
{
  "There is absolutely nothing to worry about.",
  "We are fully prepared to deal with this crisis.",
  "Our health care is fully prepared for the situation.",
  "Our country is in the midst of a great national trial, unlike any we have ever faced before.",
  "We’re at war with a deadly virus.",
  "Following the guidelines for the next 30 days is a matter of life and death",
  "This is going to be one of the roughest 2 or 3 weeks we’ve ever had in our country.",
  "We’re going to lose thousands of people.",
};
int numQuotes = sizeof( quotes ) / sizeof( const char* );

static unsigned LCG( int x )
{
  x = ( 1664525 * x + 1013904223 );
  return ( 1664525 * x + 1013904223 ) / 16;
}

static float LCG_float( int x )
{
  return LCG( x ) / 4294967295.0f;
}

void G_CoronaNewsfeed( void )
{
  static int lastShitpost = 0;
  int time, cindex, t0, p, q;
  const char *name;
  float rate, pop, cases;

  if( level.time < lastShitpost + 5000 )
  {
    return;
  }

  if( rand( ) % 100 > 2 )
  {
    return;
  }

  time = trap_RealTime( NULL ) - reference;
  cindex = rand( ) % numCountries;
  name = countries[ cindex ];

  // Roll the stats (this is actually deterministic)
  t0 = LCG( cindex ) % 5400 + 3600;
  rate = LCG_float( cindex );
  pop = 500000.0f * exp(12.0f * LCG_float( cindex + 911 ) );
  cases = exp( rate * ( time - t0 ) / 30.0f );

  p = rand( ) % numPeople;
  q = rand( ) % numQuotes;

  if( cases < 2.0f || rand( ) % 5 == 0 )
  {
    trap_SendServerCommand( -1, va( "print \"%s of %s: ''%s''\n\"", people[ p ], name, quotes[ q ] ) );
  }
  else
  {
    const char *headline;
    char buffer[ 200 ];
    int digits;
    float mul;

    if( cases > pop / 2.0f && rand( ) % 3 == 0 )
    {
      trap_SendServerCommand( -1, va( "print \"Social order breaks down in %s.\n\"", name ) );
    }
    if( cases > pop / 4.0f && rand( ) % 6 == 0 )
    {
      trap_SendServerCommand( -1, va( "print \"Pretty much everyone dead in %s.\n\"", name ) );
    }

    if( rand( ) % 4 == 0 )
    {
      headline = headlinesDeaths[ rand( ) % numHeadlinesDeaths ];
      cases /= 10.0f;
    }
    else
    {
      headline = headlines[ rand( ) % numHeadlines ];
    }

    digits = floor( log10( cases ) );
    mul = pow( 10, digits - 1 );
    cases = round( cases / mul ) * mul;

    Com_sprintf( buffer, sizeof( buffer ), headline, cases, name );
    trap_SendServerCommand( -1, va( "print \"%s\n\"", buffer ) );

    if( rand( ) % 3 == 0 )
    {
      trap_SendServerCommand( -1, va( "print \"The %s reassures: ''%s''\n\"", people[ p ], quotes[ q ] ) );
    }
  }
  
  lastShitpost = level.time;
}
